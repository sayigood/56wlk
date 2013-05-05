from scrapy.spider import BaseSpider
from scrapy.selector import HtmlXPathSelector
from scrapy.http import Request
from scrapy.http import FormRequest
import re
import time
from wuliu.items import WuTongCarLineItem

class WuliuSpider(BaseSpider):
    name = "56tong"
    allowed_domains = ["56tong.com"]
    start_urls = [
    "http://www.56tong.com/index.php?layout=yunjia"
    #"http://www.56tong.com/index.php?option=com_content&view=others&layout=yunjia_detail&carrying_id=17827&Itemid=64"
    ]

    def parse(self, response):
        page_num  = 0
        page_size = 16
        total_page_num= 0

        items = []

        hxs = HtmlXPathSelector(response)         
                
        page_str = hxs.select('.//*[@id="homeyj"]/div[2]/div[6]/form/div/p/span/span[2]/text()').extract()[0]
        #print page_str
        #print re.match('.+?(\d+).*', page_str).group(1)
        p = re.compile(r'.+?(\d+).*')
        total_page_num = p.findall(page_str)[0]
        #print total_page_num

        for page_num in range(int(total_page_num)):
            start = page_num*page_size
            formdata = {"limit1" : '%d'%page_size,
                        "start1" : '%d'%start
            }

            post_req = FormRequest(
                        url = 'http://www.56tong.com/index.php?layout=yunjia',
                        formdata = formdata,
                        callback = self.parse_hub
                        )
            items.append(post_req)

        return items

    def parse_hub(self, response):
        pre_url = 'http://www.56tong.com'

        hxs = HtmlXPathSelector(response)
        links = hxs.select('.//*[@id="homeyj"]/div[2]/div[1]/div[2]/div/div[1]/a/@href').extract()

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
        hxs = HtmlXPathSelector(response)

        item = WuTongCarLineItem() 

        item['url'] = response.url
        #item['ruku_time'] = int(time.time())  
        
        trans_type = hxs.select('.//*[@id="content"]/div[1]/div[4]/p/span[2]/text()').extract()
        if len(trans_type) == 0:
             item['trans_type'] = ''
        else:
             item['trans_type'] = trans_type[0].strip()        
        
        #item['title']    = hxs.select('.//*[@id="content"]/div[1]/div[5]/div[1]/div[2]/text()').extract()[0].strip()
        item['pub_time'] = hxs.select('.//*[@id="content"]/div[1]/div[5]/div[2]/div[2]/text()').extract()[0].strip()
        item['start_place'] = hxs.select('.//*[@id="content"]/div[1]/div[5]/div[3]/div[2]/text()').extract()[0].strip()
        item['to_place']   = hxs.select('.//*[@id="content"]/div[1]/div[5]/div[4]/div[2]/text()').extract()[0].strip()
        item['price']    = hxs.select('.//*[@id="content"]/div[1]/div[5]/div[5]/div[2]/text()').extract()[0].strip()
        item['period']   = hxs.select('.//*[@id="content"]/div[1]/div[5]/div[6]/div[2]/text()').extract()[0].strip()
        item['trans_way']      = hxs.select('.//*[@id="content"]/div[1]/div[5]/div[7]/div[2]/text()').extract()[0].strip()
        remark    = hxs.select('.//*[@id="content"]/div[1]/div[5]/div[8]/div[2]//text()').extract()
        if len(remark) == 0:
             item['remark'] = ''
        else:
             item['remark'] = "".join(remark).strip()        
        
        tel   = hxs.select('.//*[@id="content"]/div[1]/div[7]/div[2]/div[2]/span[1]/text()').extract()
        if len(tel) == 0:
            item['tel'] = ''
        else:
            item['tel'] = tel[0].strip()
            
        phone_contact   = hxs.select('.//*[@id="content"]/div[1]/div[7]/div[3]/div[2]/text()').extract()
        if len(phone_contact) == 0:
            item['phone_contact'] = ''
        else:
            item['phone_contact'] = phone_contact[0].strip()
        
        contact_name    = hxs.select('.//*[@id="content"]/div[1]/div[7]/div[4]/div[2]/text()').extract()
        if len(contact_name) == 0:
             item['contact_name']  = ''
        else:
             item['contact_name'] = contact_name[0].strip()        
        
        tax = hxs.select('.//*[@id="content"]/div[1]/div[7]/div[5]/div[2]/text()').extract()
        if len(tax) == 0:
             item['tax'] = ''
        else:
             item['tax'] = tax[0].strip()      
               
        addr     = hxs.select('.//*[@id="content"]/div[1]/div[7]/div[6]/div[2]/text()').extract()
        if len(addr) == 0:
             item['addr'] = ''
        else:
             item['addr'] = addr[0].strip()    
        
        company_name = hxs.select('.//*[@id="content"]/div[1]/div[7]/div[1]/p/a/text()').extract()
        if len(company_name) == 0:
             item['company_name'] = ''
        else:
             item['company_name'] = company_name[0].strip()
             
        item['specia_lines'] = 1

        items.append(item)

        return items
