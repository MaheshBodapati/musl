struct semid_ds {
	struct ipc_perm sem_perm;
	time_t sem_otime;
	time_t sem_ctime;
	unsigned long sem_nsems;
	unsigned long  __unused3;
	unsigned long __unused4;
};
