
1��ʹ��ʱ����ͷ�ļ�log.h������libvlog.so��̬��
2��ʹ�ò���
   ��־ģ����绯��vlog_info_t *log_init(confpath, rule_name);
   	����˵������һ������confpath������־ģ���ȡ�����ļ���·��
		  �ڶ�������rule_name�����ļ�����Ӧ�Ĺ�������
  	���ؽ����vlog_info_t����ÿ��ģ���¼��־ʱ������ڲ���Ϣ
   ��¼��־������log(vlog_info, LOG_DEBUG, ...)
   	����˵������һ������vlog_infoΪlog_init�������ص�vlog_info_t�ṹ�壬
   		  �ڶ�������Ϊ��־������LOG_DEBUG,LOG_INFO,LOG_NOTICE,LOG_WARN,LOG_ERROR,LOG_FATAL
		  ����������ΪҪ��¼����־����
   ��־ģ�����ٺ�����log_close(vlog_info)
	����˵������һ������vlog_infoΪ��һ���г�ʼ����Ľ������Ϣ
   ex:
   
   	 #include "log.h"
 	 log_info_t *log_info;
  	 void testLog()
	 {
	       while(1)   
	       log(log_info, LOG_INFO, "%s-%d\n", "mytestdsfffffffffffffffffffffffffffffffffffffffff", 100);
	 }
	 void testLog2()
 	 {
	        while(1)	
		log(log_info, LOG_DEBUG, "%s-%d\n", "testLog2", getpid());
	 }
	 int main(int argc, char **argv)
	 {
	        char *msg;
	        pthread_t pid;
	        pthread_t pid2;	  
	        msg = (char*)calloc(MAX_LEN, sizeof(char));
	        log_info = log_init("vlog.conf", "default");
	        
	        pthread_create(&pid, NULL, testLog, NULL);
	        pthread_create(&pid2, NULL, testLog2, NULL);
	        
		getchar();
 		log_close(log_info);
	      
	    	return 0;
	 }               
3�������ļ�˵��
   �ɼ�����vlog.conf    

   �����ļ���3����[global][formats][rules],��������Ӧ��ֵ���������ö��������Ŀǰ���õ�ֻ��[rules]�ڵ㣬��������Ŀǰδ��ɣ������������Ҫ�������ټ�

   [rules]�ڵ�˵��
   ���ܣ�����һ����־��¼����

   ��ϸ�����������й�������
   default.debug	>stdout|"vlog.log",20M;default

   ����defaultΪ��������debug����ɼ�¼�����־�ļ��������������.�ŷָ�������ʱֻ��Ҫ��debug�ĳ�notice,error,fatal...�ȼ��ɣ�
   �����ÿո����tab�Ʊ���ֿ��������ݣ����뷽ʽ�������ʽ��

   >stdout|"vlog.log",20M;default
  
   ��������������ʽ�������ʽ������֮ǰ��;�ŷָ�
   >stdout|"vlog.log",20MΪ��־�����ʽ����־�����ʽ�����óɶ��֣����ַ�ʽ��|�ŷָ���Ŀǰ֧�����֣�һ��std���stdout/stderr��������ļ�����������ʹ���
   ���ַ�ʽstdout���ļ���ʽ������std����ǰ��>�ű�ʶ���ļ���ʽ��""�Ŵ����ļ������ļ�������,�Ž��ļ��ɴ洢�Ĵ�С��������С�������µ���־�ļ���

   ex:
	#format
	[formats]
	default	"%d[%F %X.%ms] [%V] [%P] (%c:%f:%l)-%m%n"

	#rules
	[rules]
	*.debug	"/var/log/vlog.log",20M;format

	default.debug	>stdout|"vlog.log",20M;default
	default2.debug	>stdout|"vlog.log",20M;default

    ���õĹ����д�����~~