#include <sys/types.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <err.h>
#include <event.h>
#include <evhttp.h>
#include <string.h>
#include <stdio.h>
void generic_handler(struct evhttp_request *req, void *arg)
{
	struct evbuffer *buf;
	struct evkeyvalq *input_headers;
        struct evkeyval *header;
	buf = evbuffer_new();
	if (buf == NULL)
		err(1, "failed to create response buffer");
	input_headers = req->input_headers;
	
        TAILQ_FOREACH(header, input_headers, next) {
			printf("%s :", header->key );
			printf("%s \n", header->value );
			evbuffer_add_printf(buf,system("/bin/ls > tmpout"));				
        }
	evbuffer_add_printf(buf, "Requested: %s\n\n", evhttp_request_uri(req));
	char command[100]="/bin/cat ";
	char tmpText[500];
	char lastText[1000000]="";
	strcat(command,evhttp_request_uri(req));
	strcat(command," > tmp");
	system(command);
	FILE *tmp = fopen("tmp","r");
	int tmpState;
	while(1){
		tmpState=fgets(tmpText,sizeof(tmpText),tmp);
		if(tmpState==NULL){
			break;
		}
		strcat(lastText,"<br>");
		strcat(lastText,tmpText);	
	}
	evbuffer_add_printf(buf,lastText);				
	evhttp_add_header(req->output_headers,"Server","BoB");
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
}
int main(int argc, char **argv)
{
	struct evhttp *httpd;
	event_init();
	httpd = evhttp_start("0.0.0.0", 8080);
	/* Set a callback for requests to "/specific". */
	/* evhttp_set_cb(httpd, "/specific", another_handler, NULL); */
	/* Set a callback for all other requests. */
	evhttp_set_gencb(httpd, generic_handler, NULL);
	event_dispatch();    /* Not reached in this code as it is now. */
	evhttp_free(httpd);    return 0;
}
