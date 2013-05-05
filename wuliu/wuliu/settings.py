# Scrapy settings for wuliu project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/topics/settings.html
#

BOT_NAME = 'bd'

SPIDER_MODULES = ['wuliu.spiders']
NEWSPIDER_MODULE = 'wuliu.spiders'

#stops the wrong callback from firing
REDIRECT_ENABLED = True

# Crawl responsibly by identifying yourself (and your website) on the user-agent
#USER_AGENT = 'wuliu (+http://www.yourdomain.com)'
USER_AGENT = '%s' % (BOT_NAME)

ITEM_PIPELINES = [
        'wuliu.pipelines.WuliuPipeline'
        ]
DEFAULT_REQUEST_HEADERS={
        'UserInfo': '74432FCCC14114AA49B2B4C37E2B818625D5C63313AC023F',
        'Host': 'www.chinawutong.com',
}

LOG_FILE = 'scrapy.log'
#LOG_LEVEL = 'INFO'
LOG_LEVEL = 'DEBUG'
