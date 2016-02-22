# to return from signal handler and cleanup stack frame 
.set noreorder

.global __restore_rt
.global __restore
.type __restore_rt,@function
.type __restore,@function
__restore_rt:
__restore:
	li	$2, 5211	# __NR_rt_sigreturn,n64 and n32 have rt_sigreturn
	syscall
