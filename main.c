#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
FILE *file = NULL;
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	printf("%s\n", ptr);
    return size * nmemb;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
  curl_off_t nread;
 
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */ 
  retcode = fread(ptr, size, nmemb, file);
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
 
  return retcode;
}

int main(int argc, char *argv[])
{
	printf("%s\n", argv[1]);
	file = fopen(argv[1], "r");
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, "http://api.hivoice.cn/USCService/WebApi?appkey=c77ycb7csbqerlnpudau2rpwoxfjbadpc32jpja3&userid=guolicun@yahoo.com&id=123456");
	struct curl_slist *headers = NULL;

	//增加HTTP header
	headers = curl_slist_append(headers, "Transfer-Encoding:chunked");
//	headers = curl_slist_append(headers, "Content-Length:348160");
	headers = curl_slist_append(headers, "Content-Type:audio/x-wav;codec=pcm;bit=16;rate=16000");
	headers = curl_slist_append(headers, "Accept: text/plain");
	headers = curl_slist_append(headers, "Accept-Language:zh_CN");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
 
	//写入到文件
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	}
	sleep(2);
	curl_global_cleanup();
}		