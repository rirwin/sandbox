import feedparser
import concurrent.futures
from pprint import pprint as pprint


rss_urls = ["http://news.yahoo.com/rss/;_ylt=A0LEVzdRahtToSMAtmBXNyoA;_ylu=X3oDMTEydDN1MnZzBHNlYwNzcgRwb3MDMgRjb2xvA2JmMQR2dGlkA1FJMDQ4XzE-","http://feeds.wired.com/wired/index"]

def load_url(url):
    content = feedparser.parse(url)    
    return content

def get_titles(content):
    for i in content["items"]:
        print i["title"]

feeds_holder = []

with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
    future_to_url = {executor.submit(load_url, url): url for url in rss_urls}
    for future in concurrent.futures.as_completed(future_to_url):
        url = future_to_url[future]
        try:
            data = future.result()
            feeds_holder.append(data)
        except Exception as exc:
            print('%r generated an exception: %s' % (url, exc))
        else:
            print('%r page is %d bytes' % (url, len(data)))
    
for feed in feeds_holder:
    for entry in feed["entries"]:
        print entry.title
#    for story in items:
#        print story["title"]
    

