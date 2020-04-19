#include <stdio.h>
 #include <stdlib.h>
#include <unistd.h>
#include <string.h>  
#include "sqlite3_lib/sqlite3.h"

#include "sqlite.h"


#define DATABASE_NAME "device.db3"


 void html_body(void);
 void html_foot(void);
sqlite3 *g_gb = 0;
int main(void)
{
    printf("Set-Cookie: username=%s;\nSet-Cookie: password=%s;\n","lzm","admin");
    printf("Content-Type: text/html");
    //printf("Set-Cookie: name=lzm;");
    printf("\n\n");
    printf("<html><head>");
    printf("<meta charset=\"utf-8\">");
    printf("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    printf("<title>设备共享平台</title>");
    printf("<link rel=\"stylesheet\" href=\"/main.css\"></head>");

    html_body();

    html_foot();
    
}

void user_check(void)
{
    char *cookie_tmp = NULL;

    cookie_tmp = getenv("HTTP_COOKIE");
    if(cookie_tmp != NULL){
        printf("cookie:%s\r\n", cookie_tmp);
    }
}

void dis_rest(void)
{
    int len;
    char *r_method;
    char *http_buf=NULL;
    char *str = NULL;
    char *cookie_tmp = NULL;

    cookie_tmp = getenv("HTTP_COOKIE");
    if(cookie_tmp != NULL){
        printf("cookie:%s\r\n", cookie_tmp);
    }

    r_method=getenv("REQUEST_METHOD");
    if(r_method != NULL) {
        if(!strcmp(r_method,"POST")){

            len=atoi(getenv("CONTENT_LENGTH"));
            http_buf=(char *)malloc(len+1);
            memset((char *)http_buf,0x0,len+1);
            fgets((char *)http_buf,len+1,stdin);
            //printf("[%s]\r\n",http_buf);

            str = strstr(http_buf,"=");
            if(str)
            {
                str += 1;
                if(0 == memcmp(str,"",1))
                {
                    printf("no msg");
                    return ;
                }
                
            }
        }
        else
        {
            printf("no post:%s\r\n", r_method);
            str = getenv("QUERY_STRING");
            if(str != NULL){
                
                char *tmp = strstr(str, "id=");
                int id = -1;
                if(tmp != NULL)  id = atoi(tmp+strlen("id="));
                int use = -1;
                tmp = strstr(str, "act=");
                if(tmp != NULL)  use = atoi(tmp+strlen("act="));
                if(id >= 0 && use >= 0) {
                    printf("updata id %d use %d\r\n", id, use);
                    if(0 != access(DATABASE_NAME, F_OK|W_OK)) {
                        printf("no data\r\n\r\n");
                        return ;
                    }

                    g_gb = open_database(DATABASE_NAME );

                    update_device_isuse(g_gb, id, use);
                    close_database(g_gb);
                } 
            }
            //return;
        }

    }

    /* 默认页面 */
    if(0 != access(DATABASE_NAME, F_OK|W_OK)) {
            printf("no data\r\n\r\n");
            return ;
    }

        g_gb = open_database(DATABASE_NAME );
    printf("      <table   width=\"90%%\" id=\"mytab\"  border=\"1\" class=\"t1\"><thead><tr  class=\"a1\"><td>ID</td><td>名称</td><td>串口服务器端口</td><td>IP地址</td></tr></thead>");

    (void)select_database(g_gb, str, str, select_callback);
    close_database(g_gb);
    printf("</table>");
    

}

void html_body(void)
{
    printf("<body>");
    printf("<div id=\"container\">");
    printf("<div class=\"search bar6\">");
        printf("<form method=\"POST\" action=\"\">");
        printf("<input name=str type=\"text\" placeholder=\"请输入字符...\">");
        printf("<button type=\"submit\"></button>");
        printf("    </form>  </div>  </div> ");

        dis_rest();
}

void html_foot(void)
{
    printf("<div class=\"footer\">");
    printf("&copy;2019-2020 kikilizhm.com 版权所有 <a href=\"http://www.beian.miit.gov.cn\">苏ICP备19068650号</a></div>");
    printf("      </body></html>");
}