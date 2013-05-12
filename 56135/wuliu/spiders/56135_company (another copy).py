#-*- coding: UTF-8 -*-   
import sys
reload(sys)

sys.setdefaultencoding('utf-8') 
#----->>决文件里面不能写汉字的编码问题

from scrapy.selector import HtmlXPathSelector
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor
from scrapy.http import Request
from scrapy.contrib.spiders import CrawlSpider, Rule
import time
from wuliu.items import A56135CompanyItem

import re


class ChinaWuTongLine201Spider(CrawlSpider):
    name = '56135_company'

    allowed_domains = ['56135.com']
    start_urls=[]
    #for no in range(333400,5000000):
    #    start_urls.append('http://www.chinawutong.com/201/%d.html'%no)
    
    #start_urls = ['http://56135.com/56135/company/companylist-adv-1.html']
    start_urls = ['http://56135.com/56135/company/memberbrief/C444563.html']
    #rules = (
            #Rule(SgmlLinkExtractor(allow=r'/202/[\d]+\.html'), callback='parse_item', follow=True),
    #)
    def start_requests(self):
    	for url in self.start_urls:
        	yield Request(url, cookies={'ASP.NET_SessionId':'skyar1yam15adu55avnhajv3'})

    def parse(self, response):

	#print response.body
        items = []
        item = A56135CompanyItem()
            
        hxs = HtmlXPathSelector(response)

        item['url'] = response.url  

	contact_name    = hxs.select('.//*[@id="l_memberlink"]/text()[1]').extract() #联系人：陈聪女士
        p = re.compile(r'.*：(.*)$')
        name = p.findall(contact_name[0])
        if len(name) == 0:
             item['contact_name'] = ''
        else:
             item['contact_name'] = name[0]
             
        tel = hxs.select('.//*[@id="l_memberlink"]/font[1]/b/text()').extract()
        if len(tel[0].strip()) == 0:
             item['tel'] = ''
        else:
             item['tel'] = tel[0].strip()
             
        phone_contact = hxs.select('.//*[@id="l_memberlink"]/font[2]/b/text()').extract()
        if len(phone_contact[0].strip()) == 0:
             item['phone_contact'] = ''
        else:
             item['phone_contact'] = phone_contact[0].strip()

	mail = hxs.select('.//*[@id="l_memberlink"]/a[1]/text()').extract()
        if len(mail[0].strip()) == 0:
             item['mail'] = ''
        else:
             item['mail'] = mail[0].strip()

	im =  hxs.select('.//*[@id="l_memberlink"]/text()[7]').extract() #QQ:23432
        if len(im[0].strip()) == 0:
             item['im'] = ''
        else:
             item['im'] = im[0].strip()

        company_name = hxs.select('.//*[@id="CompanyName1_l_no"]/text()').extract()
        if len(company_name[0].strip()) == 0:
             item['company_name'] = ''
        else:
             item['company_name'] = company_name[0].strip()

	addr = hxs.select('.//*[@id="l_addr"]/text()').extract()
        if len(addr[0].strip()) == 0:
             item['addr'] = ''
        else:
             item['addr'] = addr[0].strip()
	
        remark    = hxs.select('.//*[@id="l_introduce"]//text()').extract()
        if len(remark[0].strip()) == 0:
             item['remark'] = ''
        else:
             item['remark'] = "".join(remark).strip()
                
        items.append(item)

        return items

