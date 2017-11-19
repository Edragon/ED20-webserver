#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import sys
#import struct
import select
import json
import urllib2
import thread
from coordTransform import wgs84_to_gcj02

www = '/var/www/html/2503/gps'
def amap(x, y, id, tm, nums, loc_type):
    # AMP, www.gaode.com
    
    # to GCJ02 data
    gpsDATA = wgs84_to_gcj02(x, y)

    # this is user GPS html address 
    f = open('%s/a-%s.html' % (www, id),'w')
    # this is API used in http --- > 1591e0082d616c531342592a541cc091

    amap_page = """
    <!doctype html>
    <html>
    <head>

        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="initial-scale=1.0, user-scalable=no, width=device-width">
        <title>AMAP Demo</title>
        <link rel="stylesheet" href="http://iot.electrodragon.com/gps/2503/amap.css"/>
        <script src="http://webapi.amap.com/maps?v=1.4.0&key=1591e0082d616c531342592a541cc091"></script>
        <script type="text/javascript" src="http://cache.amap.com/lbs/static/addToolbar.js"></script>
    </head>

    <body>
    <h3>My Electrodragon Tracker Demo (AMP-gaode.com API)</h3>
    <h5>Module send times: %s, module time: %s, location type %s </h5>
    <h5>Location data %s - %s (GPS -> GCJ02 version fix)</h5>
    <div id="container"></div>

    <script>
        var map = new AMap.Map('container', {
            resizeEnable: true,
            center: [%s, %s],
            zoom: 20
        });
        var marker = new AMap.Marker( {
            map: map,
            position: [%s, %s]
        });

    </script>

    </body>
    </html>
    """ % ( str(nums), str(tm), loc_type, str(gpsDATA[0]), str(gpsDATA[1]), \
    
    str(gpsDATA[0]), str(gpsDATA[1]), str(gpsDATA[0]), str(gpsDATA[1]) )
    
    f.writelines(amap_page)
    print ("seccessfully write to a-%s.html" % id )
    f.close()

def gmap(x, y, id, tm, nums, loc_type):
    # this is user GPS html address 
    f = open('%s/g-%s.html' % (www, id),'w')
    # this is API used in http --- > AIzaSyDwTjLo9c8HjFhTLyApuFc_8IIehFQDSRg

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
        <h5>Module send times: %s, module time: %s, location type %s </h5>
        <h5>Location data %s - %s </h5>

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
    """ % ( str(nums), str(tm), loc_type, str(y), str(x), str(y), str(x) )
    
    f.writelines(gmap_page)
    print ("seccessfully write to g-%s html" % id)
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
    # this http data send to thingsboard, use json data format
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

def findDATA(DATA, codec_data):
    result = codec_data.find(DATA) # find define for data
    hexdata_1 = codec_data[result+8:result+16] # 8 length string
    hexdata_2 = codec_data[result+16:result+24] # next 8 length
    return hexdata_1, hexdata_2

def hexDEC(HEXdata): 
    # hex string to decimal 
    decdata_1 = int(HEXdata[0],16)
    decdata_2 = int(HEXdata[1],16)
    return decdata_1, decdata_2


def restDATA(codec_data): 
    # get rest data except lbs location data
    hex_time = findDATA("00040006", codec_data) # 12 long
    hex_ID = findDATA("a0950006", codec_data) # 12 long
    hex_sn = findDATA("00010006", codec_data) # 12 long

    hex_sn_format = hex_sn[0]+hex_sn[1][0:4]
    hex_tm = "%s-%s%s" % (hex_time[0][0:6], hex_time[0][6:8], hex_time[1][0:4])
    pcktimes = int(codec_data[16:20],16)

    print ("HEX DATE 3L- Y-N-D: %s" % hex_time[0][0:6])
    print ("HEX TIME 3L - H-M-S: %s" % hex_time[0][6:8]+hex_time[1][0:4])
    print ("HEX pro_ID 3L: %s" % hex_ID[0]+hex_ID[1][0:4] )
    print ("HEX user_SN 3L: %s" % hex_sn_format ) 
    
    return hex_sn_format, hex_tm, pcktimes


def loc_data(codec_data): 
    # 16 length , location data
    if codec_data.find("51400008") != -1:
        Data_type = "LBS"
        DATA = findDATA("51400008", codec_data) # find flag for lbs data 51400008
    elif codec_data.find("50780008") != -1:
        Data_type = "GPS"
        DATA = findDATA("50780008", codec_data) # find flag for lbs data 51400008

    temp_lon = DATA[1]  # longtitude X
    temp_lat = DATA[0] # latitue Y
    print ("HEX %s 2x2L - X-Y: %s, %s" % (Data_type, temp_lon, temp_lat))

    decDATA = hexDEC(DATA)
    # divide to readable data
    lat = decDATA[1]/float(1000000.00)
    lon = decDATA[0]/float(1000000.00)
            
    print ("DEC %s 2x2L - X-Y: %s, %s" % (Data_type, lon, lat))

    httpsend(lon, lat)

    return lon, lat, Data_type


def DATA_dandler(codec_data):
    # handle main gps data
    locDATA = loc_data(codec_data)
    # handle rest data
    rest_data = restDATA(codec_data)
    # send to html page
    gmap(locDATA[0], locDATA[1], rest_data[0], rest_data[1], rest_data[2], locDATA[2])
    amap(locDATA[0], locDATA[1], rest_data[0], rest_data[1], rest_data[2], locDATA[2])


def on_new_client(clientsocket,addr):
    while True:
        data = clientsocket.recv(182) #182

        codec_data = data.encode('hex')  
        print codec_data
        if codec_data[0:4] == '7e7e':
            print ("INFO: star_char-%s, version-%s, action-%s, pack_times-%s" % (codec_data[0:4], codec_data[4:12], codec_data[12:16], codec_data[16:20]) ) # simple print demo
            DATA_dandler(codec_data)
            clientsocket.send("Received at Electrodragon tracker")
        else:
            clientsocket.send("you come from telnet?")
        
    clientsocket.close()


    
while True:
    conn, addr = s.accept()
    print 'Connected by ', addr
    thread.start_new_thread(on_new_client, (conn, addr))






"""
while True:
    is_readable = [s]
    is_writable = []
    is_error = []
    r, w, e = select.select(is_readable, is_writable, is_error, 1.0)

    if r:
        conn, addr = s.accept()
        print 'Connected by ', addr
        while True:    
            data = conn.recv(182) #182

            codec_data = data.encode('hex')  
            print codec_data
            print ("INFO: star_char-%s, version-%s, action-%s, pack_times-%s" % (codec_data[0:4], codec_data[4:12], codec_data[12:16], codec_data[16:20]) ) # simple print demo
            DATA_dandler()

            conn.send("server received you message.")
    else:
        print "still waiting"
        
"""
        
        
