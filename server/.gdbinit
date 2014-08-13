set print element 0
#shell sh /home/qiao/bin/kill_server.sh
define i_set
set follow-fork-mode child
catch exec
end
b remote_login
document i_set
gdb一些默认设置
end

define p_ldpc 
set $_i=0
set $_j=0
while($_i<32)
	printf "\n"
	while($_j<64)
		printf "%d|", $arg0[$_i][$_j]
		set $_j++ 
	end
	set $_i++
	set $_j=0
end
document p_ldpc
打印处LDPC_H矩阵中的数据
注意：
（1）变量的定义$_i,$_j
（2）printf "%d ", LDPC_H[$_i][$_j] 中间的逗号
（3）函数定义的格式
end

