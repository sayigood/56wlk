#-*- coding: UTF-8 -*-   
import sys
import codecs
reload(sys)

sys.setdefaultencoding('utf-8') 
#----->>决文件里面不能写汉字的编码问题

from scrapy.selector import HtmlXPathSelector
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor
from scrapy.http import Request
from scrapy.contrib.spiders import CrawlSpider, Rule
import time
import json
from wuliu.items import A56JsonItem

import re


class A56JsonSpider(CrawlSpider):
    name = '56json'

    range_up = 10

    allowed_domains = ['http://42.121.78.164']
    start_urls=[]
    for no in range(1, range_up):
        start_urls.append('http://42.121.78.164/AppServer/InfoPrepare.php?latest=0&pageSize=100&page=%d'%no)
    #start_urls.append('http://42.121.78.164/AppServer/InfoPrepare.php?latest=0&pageSize=10&page=1')
        #start_urls.append('http://42.121.78.164/AppServer/InfoPrepare.php?latest=0&pageSize=100&page=%d&maxdate=55579824'%no)
    
    #start_urls = ['http://42.121.78.164/AppServer/InfoPrepare.php?pageSize=100&page=1']
    #rules = (
            #Rule(SgmlLinkExtractor(allow=r'/202/[\d]+\.html'), callback='parse_item', follow=True),
    #)
    line = open("time.record", "r+")
    last_pubdate = int(line.readline())

    cur_maxtime = last_pubdate #init
    cnt = 0

    def parse(self, response):
        items = []        

        rb = response.body
        if rb[:3] == codecs.BOM_UTF8:
            rb = rb[3:]
        infos = json.loads(rb)['infos']
        
        for info in infos:
            item  = A56JsonItem()
            #item["aid"] = info["aid"]
            #item["channel"] = info["channel"]
            #item["title"] = info["title"]          #从哪到哪
            item["pubdate"] = info["pubdate"]      #发布时间

            if ( self.last_pubdate >= int(item["pubdate"]) ):
                #line.close()
                break
            if ( self.cur_maxtime < int(item["pubdate"]) ):
                self.cur_maxtime = int(item["pubdate"])

            #item["mid"] = info["mid"]
            #item["userid"] = info["userid"]
            item["userip"] = info["userip"]        #用户ip
            item["fromprovince"] = info["fromprovince"] #出发地-省
            item["fromcity"] = info["fromcity"]         #出发地-市
            item["fromcounty"] = info["fromcounty"]     #出发地-国家
            item["toprovince"] = info["toprovince"]     #目的地-省
            item["tocity"] = info["tocity"]             #目的地-市
            item["tocounty"] = info["tocounty"]         #目的地-国家
            item["goodtype"] = info["goodtype"]         #货物类型
            item["goodamount"] = info["goodamount"]     #货物重量
            item["fee"] = info["fee"]                   #费用
            item["carload"] = info["carload"]           #载重
            item["carnumber"] = info["carnumber"]       #车牌号
            item["cartype"] = info["cartype"]           #车类型
            item["carlong"] = info["carlong"]           #车长
            item["linkman"] = info["linkman"]           #联系人
            item["phone"] = info["phone"]               #手机号码
            item["datafrom"] = "56json"                     #来源站点 
            #item["state"] = info["state"]
            #item["mark"] = info["mark"]
            items.append(item)

        self.cnt += 1
        if (self.cnt == self.range_up-1):
            self.line.seek(0)
            self.line.write(str(self.cur_maxtime))
            self.line.close()

        return items
        #return infos
