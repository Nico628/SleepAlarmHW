 #!/usr/bin/env python   
      
import time
import serial
import os
import subprocess
import requests
import json
import random

#from selenium.webdriver import *;

import webbrowser
               
ser = serial.Serial(            
    port='/dev/serial0',
    baudrate = 9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1

 )

print('program started')
while 1:
    
    while (ser.in_waiting > 0): #Line could stil have problems maybe
        ser.flushInput()
        ser.flushOutput()
        time.sleep(.1)
    
        try:
            #incoming = ser.readline().decode("utf-8")
            #print (incoming)
            print("Opening link")
            
            
            #################################STUFF I ADDED####################################################
            
            #Get the urls from the db
            
            
            #Get the urls from the db
            try:
                songs = requests.get('http://django-env.qgbzkwbhei.us-west-1.elasticbeanstalk.com/songs/', auth=('admin', 'cpen391'), data = {})
                urls = songs.json()

                 #get the number of urls
                num_songs = requests.get('http://django-env.qgbzkwbhei.us-west-1.elasticbeanstalk.com/song-summary/', auth=('admin', 'cpen391'), data = {})
                nums = json.loads(num_songs.text)
                num = nums["song_count"]
                print(num)
                
                if num == 0 or num == 1:
                    link = "https://www.youtube.com/watch?v=a3mrxWSmd_s"
                else:
                    #take a random number % url_number and select that index in the array to be link
                    index = random.randint(0,num-1)
                    
                    #assign the link to correct youtube link!
                    link = urls[index]["youtube_link"]

            except requests.exceptions.RequestException as e:
                link = "https://www.youtube.com/watch?v=a3mrxWSmd_s"


            subproc_string = "chromium-browser " + link
                        

            p = subprocess.Popen(subproc_string.split())#shell=True)
            
            
            time.sleep(40)
                        
            p.terminate()
                        
            p.kill()
        
            
            '''
            songs = requests.get('http://django-env.qgbzkwbhei.us-west-1.elasticbeanstalk.com/songs/', auth=('admin', 'cpen391'), data = {})
            urls = songs.json()

             #get the number of urls
            num_songs = requests.get('http://django-env.qgbzkwbhei.us-west-1.elasticbeanstalk.com/song-summary/', auth=('admin', 'cpen391'), data = {})
            nums = json.loads(num_songs.text)
            num = nums["song_count"]

            #take a random number % url_number and select that index in the array to be link
            index = random.randint(0,num-1)

            #assign the link to correct youtube link!
            link = urls[index]["youtube_link"]
            
            subproc_string = "chromium-browser " + link
            
            p = subprocess.call(subproc_string, shell=True)
#            
#            time.sleep(10)
#            
#            p.terminate()
#            
#            p.kill()'''
#           
            
            
        except serial.serialutil.SerialException:
            print("No data this time")
            time.sleep(1)
            
    
