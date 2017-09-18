#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import sys
import struct
#import select
import json
import urllib2

def gmap(x, y, id):
    #write_data = "%s, %s" % (str(y), str(x))
    #print write_data
    f = open('/var/www/html/gps/%s.html' % id,'w')
    # API - AIzaSyDwTjLo9c8HjFhTLyApuFc_8IIehFQDSRg
        
    gmap_page = """
    <!DOCTYPE html>
    <html>
    <head>
        <style>
        #map {
            height: 800px;
            width: 100%%;
        }
        </style>
    </head>
    <body>
        <h3>My Electrodragon Tracker Demo</h3>
        <div id="map"></div>
        <script>
        function initMap() {
            var uluru = {lat: %s, lng: %s};
            var map = new google.maps.Map(document.getElementById('map'), {
            zoom: 15,
            center: uluru
            });
            var marker = new google.maps.Marker({
            position: uluru,
            map: map
            });
        }
        </script>
        <script async defer
        src="https://maps.googleapis.com/maps/api/js?key=AIzaSyDwTjLo9c8HjFhTLyApuFc_8IIehFQDSRg&callback=initMap">
        </script>
    </body>
    </html>
    """ % ( str(y), str(x) )
	
    f.writelines(gmap_page)
    print ("seccessfully write")
    f.close()
    #return gmap_page

HOST = ''
PORT = 8100

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)   ## reuse socket 8100
s.bind((HOST, PORT))
s.listen(5)

print 'Server start at: %s:%s' %(HOST, PORT)
print 'wait for connection...'

def httpsend(x, y):
    data = {
           "latitude": y, 
          "longitude": x,
          "batteryLevel": "0",
          "batteryCharging": "0"

    }
    credential = "qt6GZuEqH685HYzi8OlH"
    addr = "demo.thingsboard.io:80"
    
    req = urllib2.Request('http://%s/api/v1/%s/attributes' % (addr,credential) )

    req.add_header('Content-Type', 'application/json')

    response = urllib2.urlopen(req, json.dumps(data))
    #print("data uploaded")

def findDATA(DATA):
    result = codec_data.find(DATA) # find flag for lbs data
    hexdata_1 = codec_data[result+8:result+16] #8
    hexdata_2 = codec_data[result+16:result+24]
    return hexdata_1, hexdata_2

def hexDEC(HEXdata): 
    decdata_1 = int(HEXdata[0],16)
    decdata_2 = int(HEXdata[1],16)
    return decdata_1, decdata_2


def restDATA(): 
    hex_time = findDATA("00040006") # 12 long
    hex_ID = findDATA("a0950006") # 12 long
    hex_sn = findDATA("00010006") # 12 long
    hex_sn_format = hex_sn[0]+hex_sn[1][0:4]
	
    print ("HEX DATE 3L- Y-N-D: %s" % hex_time[0][0:6])
    print ("HEX TIME 3L - H-M-S: %s" % hex_time[0][6:8]+hex_time[1][0:4])
    print ("HEX pro_ID 3L: %s" % hex_ID[0]+hex_ID[1][0:4] )
    print ("HEX user_SN 3L: %s" % hex_sn_format ) 
	
    return hex_sn_format


def lbs_data(): # 16 , if find lbs
    if codec_data.find("51400008") != -1:
        DATA = findDATA("51400008") # find flag for lbs data 51400008

        temp_lbs_lon = DATA[1]  # longtitude X
        temp_lbs_lat = DATA[0] # latitue Y
        print ("HEX LBS 2x2L - X-Y: %s, %s" % (temp_lbs_lon, temp_lbs_lat))

        decDATA = hexDEC(DATA)
        lbs_lat = decDATA[1]/float(1000000.00)
        lbs_lon = decDATA[0]/float(1000000.00)
            
        print ("DEC LBS 2x2L - X-Y: %s, %s" % (lbs_lon, lbs_lat))

        httpsend(lbs_lon, lbs_lat)

        return lbs_lon, lbs_lat
    else:
        return False


def allDATA():
    #if lbs_data():
    lbsDATA = lbs_data()
    rest_data = restDATA()
    gmap(lbsDATA[0], lbsDATA[1], rest_data)


while True:
    conn, addr = s.accept()
    print 'Connected by ', addr

    while True:
        #r, _, _ = select.select([self.conn], [], [])
        #if r:
        #    # ready to receive
            
        data = conn.recv(182) #182
        codec_data = data.encode('hex')
        print codec_data
        print ("INFO: star_char-%s, version-%s, action-%s, pack_times-%s" % (codec_data[0:4], codec_data[4:12], codec_data[12:16], codec_data[16:20]) ) # simple print demo
        allDATA()

        conn.send("server received you message.")
		
		
		
		
