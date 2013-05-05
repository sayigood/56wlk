from scrapy.selector import HtmlXPathSelector
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor
from scrapy.contrib.spiders import CrawlSpider, Rule
from wuliu.items import WuliuItem

class WeiboSpider(CrawlSpider):
    name = 'weibo'
    allowed_domains = ['weibo.com']
    start_urls = ['http://www.weibo.com/']

    rules = (
        Rule(SgmlLinkExtractor(allow=r'Items/'), callback='parse_item', follow=True),
    )

    def parse_item(self, response):
        hxs = HtmlXPathSelector(response)
        i = WuliuItem()
        #i['domain_id'] = hxs.select('//input[@id="sid"]/@value').extract()
        #i['name'] = hxs.select('//div[@id="name"]').extract()
        #i['description'] = hxs.select('//div[@id="description"]').extract()
        return i
