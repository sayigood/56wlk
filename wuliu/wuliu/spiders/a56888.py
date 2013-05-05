#-*- coding: UTF-8 -*-   
import sys
reload(sys)

sys.setdefaultencoding('utf-8') 
#----->>决文件里面不能写汉字的编码问题
from scrapy.selector import HtmlXPathSelector
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor
from scrapy.http import Request
from scrapy.contrib.spiders import CrawlSpider, Rule
import re
import time
from wuliu.items import WuTongCarLineItem

class A56888Spider(CrawlSpider):
    name = 'a56888'
    allowed_domains = ['56888.net']
    start_urls = ['http://dx88.56888.net/Line/LineList.aspx?startProvince=&startCity=&endProvince=&endCity=&Page=1']
    #start_urls = ['http://dx88.56888.net/Line/LineView.aspx?lineid=74108']
    #rules = (
            #Rule(SgmlLinkExtractor(allow=r'Items/'), callback='parse_item', follow=True),
            #)

    def parse(self, response):
        hxs = HtmlXPathSelector(response)
        items = []
        page_str = hxs.select('.//*[@id="TotalPage"]/text()').extract()[0]
        p = re.compile(r'.+?(\d+).*')
        total_page_num = p.findall(page_str)[0]

        for page_num in range(1, int(total_page_num)+1, 1):
            req = Request(
                    url = 'http://dx88.56888.net/Line/LineList.aspx?startProvince=&startCity=&endProvince=&endCity=&Page=' + str(page_num),
                    callback = self.parse_hub,
                    )
            items.append(req)

        return items

    def parse_hub(self, response):
        pre_url = "http://dx88.56888.net/Line/"
        items = []

        hxs = HtmlXPathSelector(response)
        links = hxs.select('.//*[@id="lineData"]/table/tr/td[2]/a/@href').extract()

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
        items = []
        item = WuTongCarLineItem()

        hxs = HtmlXPathSelector(response)

        item['url'] = response.url
        #item['ruku_time'] = int(time.time())  

        company_name = hxs.select('.//*[@id="line_info"]/table/tr[1]/td/text()').extract()
        if len(company_name) == 0:
             item['company_name'] = ''
        else:
             item['company_name'] = company_name[0].strip()

        contact_name = hxs.select('.//*[@id="line_info"]/table/tr[4]/td/text()').extract()
        if len(contact_name) == 0:
             item['contact_name'] = ''
        else:
             item['contact_name'] = contact_name[0].strip()

        from_to      = hxs.select('.//*[@id="line_info"]/table/tr[2]/td[1]/text()').extract()[0].strip()
        if len(from_to) == 0:
            item['start_place'] = ''
            item['to_place']   = ''
        else:
            m = re.match(r'(.*)--->(.*)', from_to)
            item['start_place'] = m.group(1)
            item['to_place']   = m.group(2)

        tel = hxs.select('.//*[@id="line_info"]/table/tr[5]/td[1]/text()').extract()
        if len(tel) == 0:
             item['tel'] = ''
        else:
             item['tel'] = tel[0].strip()

        phone_contact = hxs.select('.//*[@id="line_info"]/table/tr[5]/td[2]/text()').extract()
        if len(phone_contact) == 0:
             item['phone_contact'] = ''
        else:
             m = re.match(u"([\u4e00-\u9fa5]+)：(.*)", phone_contact[0].decode("utf-8"))
             item['phone_contact'] = m.group(2)              

        addr   = hxs.select('.//*[@id="line_info"]/table/tr[6]/td/text()').extract()
        if len(addr) == 0:
             item['addr'] = ''
        else:
             item['addr'] = addr[0].strip()

        trans_type = hxs.select('.//*[@id="line_info"]/table/tr[3]/td/text()').extract()
        if len(trans_type) == 0:
             item['trans_type'] = ''
        else:
             item['trans_type'] = trans_type[0].strip()

        remark = hxs.select('.//*[@id="line_info"]/table/tr[7]/td//text()').extract()
        if len(remark) == 0:
             item['remark'] = ''
        else:
             item['remark']   = "".join(remark).strip()
             
        item['specia_lines'] = 1

        items.append(item)

        return items

