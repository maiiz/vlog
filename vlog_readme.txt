
1、使用时引用头文件log.h，链接libvlog.so动态库
2、使用步骤
   日志模块初如化：vlog_info_t *log_init(confpath, rule_name);
   	参数说明：第一个参数confpath传入日志模块读取配置文件的路径
		  第二个参数rule_name配置文件中相应的规则名称
  	返回结果：vlog_info_t保存每个模块记录日志时所需的内部信息
   记录日志函数：log(vlog_info, LOG_DEBUG, ...)
   	参数说明：第一个参数vlog_info为log_init函数返回的vlog_info_t结构体，
   		  第二个参数为日志级别，有LOG_DEBUG,LOG_INFO,LOG_NOTICE,LOG_WARN,LOG_ERROR,LOG_FATAL
		  第三个参数为要记录的日志内容
   日志模块销毁函数：log_close(vlog_info)
	参数说明：第一个参数vlog_info为第一步中初始化后的结果体信息
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
3、配置文件说明
   可见代码vlog.conf    

   配置文件分3个节[global][formats][rules],节下是相应的值，可以配置多个，其中目前有用的只有[rules]节点，其它两个目前未完成，后续如果有需要，可以再加

   [rules]节点说明
   功能：配置一种日志记录规则

   详细描述：比如有规则如下
   default.debug	>stdout|"vlog.log",20M;default

   其中default为规则名，debug代表可记录输出日志的级别，它与规则名用.号分隔，调整时只需要把debug改成notice,error,fatal...等即可，
   基后用空格或者tab制表符分开后续内容（输入方式及输入格式）

   >stdout|"vlog.log",20M;default
  
   这个是配置输出方式与输出格式，它们之前用;号分隔
   >stdout|"vlog.log",20M为日志输出方式，日志输出方式可配置成多种，多种方式用|号分隔，目前支持两种，一个std输出stdout/stderr和输出到文件，这条规则就代表
   两种方式stdout和文件方式，其中std输入前用>号标识，文件方式用""号代表文件名，文件名后用,号接文件可存储的大小，超过大小则生成新的日志文件，

   ex:
	#format
	[formats]
	default	"%d[%F %X.%ms] [%V] [%P] (%c:%f:%l)-%m%n"

	#rules
	[rules]
	*.debug	"/var/log/vlog.log",20M;format

	default.debug	>stdout|"vlog.log",20M;default
	default2.debug	>stdout|"vlog.log",20M;default

    配置的功能有待完善~~