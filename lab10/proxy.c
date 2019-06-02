/*
 * proxy.c - ICS Web proxy
 * ID: 517030910217
 * Name: Rui Shaopu
 */

#include "csapp.h"
#include <stdarg.h>
#include <sys/select.h>

/*
 * Function prototypesb
 */
int parse_uri(char *uri, char *target_addr, char *path, char *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, size_t size);
void *thread(void *cond);
void doit(int connfd, struct sockaddr_in *sockaddr);

sem_t s;

/*
 * main - Main routine for the proxy program
 */
int main(int argc, char **argv)
{
    int listenfd;．
    char *connfdp;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    pthread_t threadid;
    struct sockaddr_storage clientaddr;
    
    Signal(SIGPIPE, SIG_IGN);
    Sem_init(&s, 0, 1);

    /* Check command-line arguments */
    if (argc != 2){
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);

	while(1){
		clientlen = sizeof(clientaddr);
		connfdp = Malloc(sizeof(int) + sizeof(uint32_t));
        *((int*)connfdp) = Accept(listenfd, (SA *)&clientaddr, &clientlen); 	
		Getnameinfo((SA*)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, NI_NUMERICHOST);
		inet_pton(AF_INET, hostname, connfdp+sizeof(int));
	
		Pthread_create(&threadid, NULL, thread, connfdp);
	}

    exit(0);
}

void *thread(void *p)
{
	Pthread_detach(pthread_self());

    struct in_addr addr;
    addr.s_addr = *((uint32_t*)(p+sizeof(int)));

	struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr = addr;

    int connfd = *((int*)p);

	free(p);
    doit(connfd, &sockaddr);
    close(connfd);
	
	return NULL;
}

void doit(int connfd, struct sockaddr_in *sockaddr)
{
    int clientfd;
	char client_buf[MAXLINE], server_buf[MAXLINE], request[MAXLINE];
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char hostname[MAXLINE], pathname[MAXLINE], port[MAXLINE];
	rio_t clientio, serverio;

	int req_bodysize = 0;
	int res_bodysize = 0, res_headsize = 0, res_tmp;
	int read_res, write_res;

 	rio_readinitb(&clientio, connfd);

	if ((read_res=rio_readlineb(&clientio, client_buf, MAXLINE)) <= 0) { 
		return;
	}

    if (sscanf(client_buf, "%s %s %s", method, uri, version)!=3) {
        return;
    }

    if (parse_uri(uri, hostname, pathname, port) != 0) {
		return;
	}

	sprintf(request, "%s /%s %s\r\n", method, pathname, version);

	while ((read_res=rio_readlineb(&clientio, client_buf, MAXLINE)) > 0) {

		if (!strncasecmp(client_buf, "Content-Length", 14)) {
			req_bodysize = atoi(client_buf+15);
		}
		sprintf(request, "%s%s", request, client_buf);

		if (!strcmp(client_buf, "\r\n")) {
			break;
		}
	}

	if ((clientfd=open_clientfd(hostname, port)) <= 0) {
		return;
	}

	rio_readinitb(&serverio, clientfd);

    write_res = rio_writen(clientfd, request, strlen(request));
	if (write_res != strlen(request)){
        close(clientfd);
		return;
	}

	if (req_bodysize != 0) {
        for(int i = 0; i < req_bodysize; i++){
			if ((read_res=rio_readnb(&clientio, client_buf, 1)) < 1) {
		        if (i + 1 == req_bodysize) {
                    close(clientfd);
                    return;
                }
			}

			if ((write_res=rio_writen(clientfd, client_buf, 1)) != 1){
    			close(clientfd);
		    	return;
			}
		}
	}

	while ((res_tmp = rio_readlineb(&serverio, server_buf, MAXLINE)) > 0) {
		if (!strncasecmp(server_buf, "Content-Length", 14)) {
			res_bodysize = atoi(server_buf+15);
		}

		res_headsize += res_tmp;

		if ((write_res=rio_writen(connfd, server_buf, strlen(server_buf))) != strlen(server_buf)) {
        	close(clientfd);
			return;
		}

		if (!strcmp(server_buf, "\r\n")){
			break;
		}
	}

	if (res_tmp <= 0){
        close(clientfd);
		return;
	}
	
    for (int i = 0; i< res_bodysize; i++) {
		if ((read_res=rio_readnb(&serverio, server_buf, 1)) <= 0) {
            if (i != res_bodysize - 1) {
                close(clientfd);
                return;
            }
		}

		if ((write_res=rio_writen(connfd, server_buf, 1)) != 1) {
        	close(clientfd);
			return;
		}
	}

	close(clientfd);

	char logstring[MAXLINE];
	format_log_entry(logstring, sockaddr, uri, res_bodysize + res_headsize);

    P(&s);
	printf("%s\n", logstring);
    V(&s);
}

/*
 * parse_uri - URI parser
 *
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *hostname, char *pathname, char *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0)
    {
        hostname[0] = '\0';
        return -1;
    }

    /* Extract the host name */
    hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    if (hostend == NULL)
        return -1;
    len = hostend - hostbegin;
    strncpy(hostname, hostbegin, len);
    hostname[len] = '\0';

    /* Extract the port number */
    if (*hostend == ':')
    {
        char *p = hostend + 1;
        while (isdigit(*p))
            *port++ = *p++;
        *port = '\0';
    }
    else
    {
        strcpy(port, "80");
    }

    /* Extract the path */
    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL)
    {
        pathname[0] = '\0';
    }
    else
    {
        pathbegin++;
        strcpy(pathname, pathbegin);
    }

    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring.
 *
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), the number of bytes
 * from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr,
                      char *uri, size_t size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /*
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 12, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;

    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s %zu", time_str, a, b, c, d, uri, size);
}
