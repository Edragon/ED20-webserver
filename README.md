## to use in background 

* buy a cheap server at ramnode, start from a flesh server
* install nginx -> apt-get install nginx, notice to stop apache if you have
* install git -> apt-get install git
* default nginx folder is /var/www/html, cd into this folder
* git clone 2503 project into /var/www/html
* run test command python /var/www/html/2503/gps/main.py
* find html file in gps folder -> 

Test 
* make sure sim inserted
* set sn $setsn xxxxxxxxx   (12 numbers)
* set ip $setip $setip 167.88.114.101    (iot2.electrodragon.com)
run -> nohup python /var/www/html/2503/gps/main.py &


## Resource
* GPS check validate - http://www.gpsspg.com/maps.htm
* chinese amap - http://lbs.amap.com
