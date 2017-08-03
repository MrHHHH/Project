/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/

#define _esp 0
#define _eip 4
/* ------ */
#define _rsp 0
#define _rip 8
#define _rbx 16
#define _rdi 24
#define _rsi 32

/* 协程上下文切换实现 */
/* c++等高级语言无法操纵寄存器，所以使用汇编实现 */

.globl coctx_swap 
.type  coctx_swap, @function
coctx_swap:

/*协程上下文切换的函数原型：
void coctx_swap( coctx_t *,coctx_t* ) asm("coctx_swap");
参数一：要切出的协程， 参数二：切出后要进入的协程
*/

/* coctx.h 文件中定义
struct coctx_t
{
	void *regs[ 5 ];

	coctx_param_t *param;

	coctx_pfn_t routine;
	const void *s1;
	const void *s2;
	size_t ss_size;
	char *ss_sp;
};
*/

//假设现在要由协程a切换到协程b
//32位
#if defined(__i386__)
	/*执行之前，函数先将两个参数和返回地址压栈*/
	leal -4(%esp),%esp
	pushl %ebx
	pushl %ecx
	pushl %edx

	pushl %edi
	pushl %esi
	pushl %ebp
	leal 28(%esp),%esp //esp指向

	/* sp,ip */
	/* param old ctx */
	movl 4(%esp), %eax  //eax存储协程a传进来的coctx_t*

	/*保存要切出协程a的运行状态*/
	movl (%esp), %ecx  //ecx存储a的返回地址
	movl %ecx, _eip(%eax) //将a的返回地址放入a的regs[1]

	leal 4(%esp), %ecx //ecx存储函数开始压参时第一个参数a在内存中的地址，相当于协程a的栈顶esp。
	movl %ecx, _esp(%eax) //将a的esp放入a的regs[0]

	/* param new ctx */
	movl 8(%esp), %eax  //eax存储协程b传进来的coctx_t*
	
	/* push ret ( ip,sp ) */
	/*将要切入的协程a的状态恢复*/
	movl _eip(%eax), %ecx //将b的regs[1]即返回地址，放入ecx中
	movl _esp(%eax), %esp //将b的regs[0]即协程b的esp，放入esp中
	pushl %ecx   //将协程b的返回地址入栈

	leal -28(%esp),%esp
	popl %ebp
	popl %esi
	popl %edi

	popl %edx
	popl %ecx
	popl %ebx
	leal 4(%esp),%esp //esp指向协程b的返回地址，就是上面push进去的
	
	movl $0, %eax
	ret  //call b的返回地址  跳转到b协程执行
	//利用ret命令修改指令寄存器 %eip ，通过修改 %eip 来实现程序运行逻辑跳转。

#elif defined(__x86_64__)

	leaq -8(%rsp),%rsp
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rdx
	pushq %rcx	
	pushq %r8
	pushq %r9
	leaq 80(%rsp),%rsp	

	movq %rbx,_rbx(%rdi)
	movq %rdi,_rdi(%rdi)
	movq %rsi,_rsi(%rdi)
	/* sp */
	movq (%rsp), %rcx
	movq %rcx, _rip(%rdi)
	leaq 8(%rsp), %rcx		
	movq %rcx, _rsp(%rdi)

	/* sp */
	movq _rip(%rsi), %rcx
	movq _rsp(%rsi), %rsp
	pushq %rcx

	movq _rbx(%rsi),%rbx
	movq _rdi(%rsi),%rdi
	movq _rsi(%rsi),%rsi

	leaq -80(%rsp),%rsp
	popq %r9
	popq %r8
	popq %rcx	
	popq %rdx
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	leaq 8(%rsp),%rsp	

	xorl %eax, %eax
	ret
#endif
