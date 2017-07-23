<html lang="en"><head>
    <meta charset="UTF-8">
<body><h1>多线程web服务器</h1>
<p>服务器能够通过监听本地8080端口对指定的请求做出响应<br><br>
开发语言：纯c语言  

</p>
<p>开发环境：centos

</p>
<p>功能：  

</p>
<pre><code>  1. 使用socket编程创建tcp连接
  2. 服务器采用多线程处理每个连接  
  3. 服务器接受http请求并分析http报头进行处理后返回一个http响应
  4. 支持cgi功能，能够在服务器端动态处理请求</code></pre>
</body></html>
