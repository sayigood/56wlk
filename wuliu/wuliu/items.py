#-*- coding: UTF-8 -*-   
import sys
reload(sys)

sys.setdefaultencoding('utf-8')
#----->>决文件里面不能写汉字的编码问题
# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/topics/items.html

from scrapy.item import Item, Field



class WuliuItem(Item):
    # define the fields for your item here like:
    # name = Field()
    url = Field()
    cur_time = Field()
    title= Field()
    pub_time = Field()
    from_pos = Field()
    to_pos = Field()
    price= Field()
    time = Field()
    way= Field()
    hphone = Field()
    mphone = Field()
    name = Field()
    addr = Field()
    pass

#class A56888Item(Item):
    #url = Field()
    #cur_time = Field()
    #company= Field()
    #name = Field()
    #from_pos = Field()
    #to_pos = Field()
    #hphone = Field()
    #mphone = Field()
    #addr = Field()
    #trans_way= Field()
    #desc = Field()
    #pass

class WuTongCarLineItem(Item):  
    trans_way = Field()     #货运方式，单程or往返
    trans_type = Field()    #运输方式，海运or货运
    period    = Field()     #货运周期
    price     = Field()     #运价
    addr      = Field()     #地址
    tax       = Field()     #传真
    im        = Field()     #qq/msn
    truck_type = Field()    #车型
    
    start_place = Field()   #出发地
    to_place = Field()      #目的地
    company_name = Field()  #企业名
    contact_name= Field()   #联系人姓名
    phone_contact = Field() #手机
    tel = Field()           #电话
    models= Field()         #车体类型,	厢式or高栏
    license_plate = Field() #车牌
    truck_long = Field()    #车长
    load = Field()          #载重
    start_time = Field()    #发车时间
    specia_lines = Field()  #专线标记
    pub_time = Field()      #发布时间
    click_count = Field()   #点击数
    ruku_time = Field()     #入库时间
    url = Field()           #来源网站
    remark = Field()        #备注
    pass


#class WuTongLine201Item(Item):
    #url = Field()
    #cur_time= Field()
    #pub_name= Field()
    #from_pos= Field()
    #from_name = Field()
    #from_hphone = Field()
    #from_mphone = Field()
    #from_im = Field()
    #from_addr = Field()

    #to_pos= Field()
    #to_name = Field()
    #to_hphone = Field()
    #to_mphone = Field()
    #to_im = Field()
    #to_addr = Field()

    #company = Field()
    #desc= Field()
    #pass
