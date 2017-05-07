#/bin/bash/

#控制httpd启停的控制脚本

ROOT_PATH=$(pwd)
BIN=${ROOT_PATH}/httpd
CONF=${ROOT_PATH}/conf/httpd.conf
PID=${ROOT_PATH}/httpd.pid

ctl=$(basename $0)
function Usage()
{
	printf "Usage: %s start(-s) | stop (-t) | restart(-rs)\n" "${ctl}"
}

function Start()
{
	if [ -f $PID ]; then
		printf "start failed!!! http is runing, pid is $(cat ${PID}).\n"
		return
	fi
	ip=$(grep -E 'IP:' $CONF | awk -F: '{print $2}')
	port=$(grep -E 'PORT:' $CONF | awk -F: '{print $2}')
	$BIN $ip $port
	echo $ip
	echo $port
	pidof $(basename $BIN) > $PID
	printf "start success!!! pid is $(cat $PID)\n"
}

function Stop()
{
	if [ ! -f $PID ]; then
		printf "stop failed!!! No httpd is runing.\n"
		return
	fi
	
	id=$(cat $PID)
	kill -9 $id
	rm -f $PID
	rm -f ./log/httpd.log
	printf "stop success!!! httpd is stoped. pid is $id\n"
}

#检查命令行参数
if [ $# -ne 1 ] ; then
	Usage
	exit 1
fi


case $1 in
	start | -s)
		Start
	;;
	stop | -t)
		Stop
	;;
	restart | -rs)
		Stop
		Start
	;;
	*)
		Usage
		exit 2
	;;
esac
	
