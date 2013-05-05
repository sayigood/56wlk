#-*- coding: UTF-8 -*-   
import sys
reload(sys)

sys.setdefaultencoding('utf-8') 
#----->>决文件里面不能写汉字的编码问题

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/topics/item-pipeline.html

#class WuliuPipeline(object):
##    def __init__(self):
##       self.file = open('result.txt','w')

    #def process_item(self, item, spider):
##        self.file.write(urllib2.unquote(item['title'].strip().encode('gbk')))
        ##self.file.write(item['title'])
##        self.file.write('\n')
        #return item

from scrapy import log
#from scrapy.core.exceptions import DropItem
from twisted.enterprise import adbapi
from scrapy.http import Request
from scrapy.exceptions import DropItem
#from scrapy.contrib.pipeline.images import ImagesPipeline
import time
import MySQLdb
import MySQLdb.cursors
import urllib2

class WuliuPipeline(object):

    def __init__(self):
        self.dbpool = adbapi.ConnectionPool(
                "MySQLdb",
                db = 'chemanman',
                user = 'root',
                passwd = 'root',
                host='localhost',
                cursorclass = MySQLdb.cursors.DictCursor,
                charset = 'utf8',
                use_unicode = False
        )

        #self.mysql = adbapi.ConnectionPool(
            #"MySQLdb",
            #db=self.db,
            #port=self.port,
            #user=self.user,
            #passwd=self.passwd,
            #host=self.host,
            #cp_reconnect=True,
            #cursorclass=DictCursor)


    def process_item(self, item, spider):
        
        query = self.dbpool.runInteraction(self._conditional_insert, item)
        
        query.addErrback(self.handle_error)
        return item
  
    def _conditional_insert(self, tx, item):
        fields = "`"
        fields += "`, `".join(item.keys())
        fields += "`"
        
        values = ''
        for v in item.values():
            if len(values) > 0:
                values += ", \'%s\'"%v
            else:                
                values += "\'%s\'"%v

        sql = "insert into goodsource_table (%s) values (%s)"%(fields, values)
        log.err(sql)
        tx.execute(sql)
        
    
    def handle_error(self, e):
        log.err(e)
