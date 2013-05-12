from scrapy.selector import HtmlXPathSelector
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor
from scrapy.http import Request
from scrapy.contrib.spiders import CrawlSpider, Rule
import time
from wuliu.items import WuTongCarLineItem

import re

class ChinaWuTongCar102Spider(CrawlSpider):
    name = 'chinawutong_car102'

    allowed_domains = ['chinawutong.com']
    start_urls=[]
    for no in range(1,7000):
        start_urls.append('http://www.chinawutong.com/102.html?pid=%d'%no)
        
    #start_urls = ['http://www.chinawutong.com/102.html?pid=3']
    #start_urls = ['http://www.chinawutong.com/202/14493.html']
    #start_urls = ['http://www.chinawutong.com/202/498871.html','http://www.chinawutong.com/202/498871.html','http://www.chinawutong.com/202/498871.html']
    #print start_urls
    #rules = (
            #Rule(SgmlLinkExtractor(allow=r'/202/[\d]+\.html'), callback='parse_item', follow=True),
            #)

    def parse(self, response):
        pre_url = "http://www.chinawutong.com"
        items = []

        hxs = HtmlXPathSelector(response)
        links = hxs.select('.//*[@id="aspnetForm"]/div[12]/div[1]/div[6]/div/div/p[1]/a/@href').extract()
        #print links
        items = []
        for link in links:
            req = Request(
                    url = pre_url + link,
                    #url = pre_url + link,
                    callback = self.parse_detail,
                    )
            items.append(req)

        return items

    def parse_detail(self, response):
    #def parse(self, response):
        items = []
        item = WuTongCarLineItem()
        
        #print response.status
            
        hxs = HtmlXPathSelector(response)

        item['url'] = response.url
        #item['ruku_time'] = int(time.time())  
        
        license_plate  = hxs.select('.//*[@id="ctl00_cphView_fvCarLine"]/tr/td/table/tr[1]/td[2]/text()').extract()
        if len(license_plate) == 0:
             item['license_plate'] = ''
        else:
             item['license_plate'] = license_plate[0].strip()
             
        trans_way     = hxs.select('.//*[@id="ctl00_cphView_fvCarLine"]/tr/td/table/tr[2]/td[2]/text()').extract()
        if len(trans_way) == 0:
             item['trans_way'] = ''
        else:
             item['trans_way'] = trans_way[0].strip()
    
        start_place = hxs.select('.//*[@id="ctl00_cphView_fvCarLine"]/tr/td/table/tr[3]/td[2]/a/text()').extract()
        if len(start_place) == 0:
             item['start_place'] = ''
        else:
             item['start_place'] = "-".join(start_place).strip()
    
        to_place   = hxs.select('.//*[@id="ctl00_cphView_fvCarLine"]/tr/td/table/tr[4]/td[2]/a/text()').extract()
        if len(to_place) == 0:
             item['to_place'] = ''
        else:
             item['to_place'] = "-".join(to_place).strip()
    
        #jingguo  = hxs.select('.//*[@id="ctl00_cphView_fvCarLine"]/tr/td/table/tr[5]/td[2]/text()').extract()
        #if len(jingguo) == 0:
             #item['jingguo'] = ''
        #else:
             #item['jingguo'] = jingguo[0].strip()
    
        tel   = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[8]/td[2]/text()').extract()
        if len(tel) == 0:
             item['tel'] = ''
        else:
             item['tel'] = tel[0].strip()
    
        phone_contact   = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[9]/td[2]/text()').extract()
        if len(phone_contact) == 0:
             item['phone_contact'] = ''
        else:
             item['phone_contact'] = phone_contact[0].strip()
    
        pub_time = hxs.select('.//*[@id="ctl00_cphView_fvCarLine"]/tr/td/table/tr[6]/td[2]/text()').extract()
        if len(pub_time) == 0:
             item['pub_time'] = ''
        else:
             item['pub_time'] = pub_time[0].strip()
    
        start_time = hxs.select('.//*[@id="ctl00_cphView_fvCarLine"]/tr/td/table/tr[7]/td[2]/text()').extract()
        if len(start_time) == 0:
             item['start_time'] = ''
        else:
             item['start_time'] = start_time[0].strip()
 
        remark  = hxs.select('.//*[@id="ctl00_cphView_fvCarLine"]/tr/td/table/tr[8]/td[2]/text()').extract()
        if len(remark) == 0:
             item['remark'] = ''
        else:
             item['remark'] = remark[0].strip() 
    
        truck_long     = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[2]/td[2]/text()').extract()
        if len(truck_long) == 0:
             item['truck_long'] = ''
        else:
             item['truck_long'] = truck_long[0].strip()
    
        load = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[3]/td[2]/text()').extract()
        if len(load) == 0:
             item['load'] = ''
        else:
             item['load'] = load[0].strip()
    
        addr     = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[4]/td[2]/text()').extract()
        if len(addr) == 0:
             item['addr'] = ''
        else:
             item['addr'] = addr[0].strip()
    
        truck_type    = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[5]/td[2]/text()').extract()
        if len(truck_type) == 0:
             item['truck_type'] = ''
        else:
             item['truck_type'] = truck_type[0].strip()
    
        models  = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[6]/td[2]/text()').extract()
        if len(models) == 0:
             item['models'] = ''
        else:
             item['models'] = models[0].strip()
    
        contact_name    = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[7]/td[2]/text()').extract()
        if len(contact_name) == 0:
             item['contact_name'] = ''
        else:
             item['contact_name'] = contact_name[0].strip()
             
             
        item['specia_lines'] = 0
    
        #car_id  = hxs.select('.//*[@id="ctl00_cphView_fvCar"]/tr/td/table/tr[10]/td[2]/text()').extract()
        #if len(car_id) == 0:
             #item['car_id'] = ''
        #else:
             #item['car_id'] = car_id[0].strip()
    

        items.append(item)

        return items

