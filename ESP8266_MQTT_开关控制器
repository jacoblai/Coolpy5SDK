	print("Coolpy5 V1.0 ESP8266_SDK MQTT")
	server = "192.168.1.105"--Coolpy5所在路由器或服务器的域名或IP地址
	port = 1883--酷痞平台mqtt服务端口
	ukey = "51dd40af-135f-4733-89ff-a07615282812"--UserKey用户密钥(必改项)
	hub = 5--Hub ID(必改项)
	cnode = 5--Node ID(必改项)
	--(让芯片连接互联网)
	wifiPoint = "YMS_805_1"--本地路由器热点名
	wifiPwd = "yms*****"--本地路由器密码
    
	local str=wifi.ap.getmac();
    local ssidTemp=string.format("%s%s%s",string.sub(str,10,11),string.sub(str,13,14),string.sub(str,16,17));
    wifi.setmode(wifi.STATIONAP)
    
    local cfg={}
    cfg.ssid="ESP8266_"..ssidTemp;
    cfg.pwd="12345678"
    wifi.ap.config(cfg)
     cfg={}
     cfg.ip="192.168.4.1";
     cfg.netmask="255.255.255.0";
     cfg.gateway="192.168.4.1";
     wifi.ap.setip(cfg);
     
     wifi.sta.config(wifiPoint,wifiPwd)
     wifi.sta.connect()
     
     local cnt = 0
     gpio.mode(0,gpio.OUTPUT);
     tmr.alarm(0, 1000, 1, function() 
         if (wifi.sta.getip() == nil) and (cnt < 20) then 
             print("Trying Connect to Router, Waiting...")
             cnt = cnt + 1 
                  if cnt%2==1 then gpio.write(0,gpio.HIGH);
                  else gpio.write(0,gpio.LOW); end
         else 
             tmr.stop(0);
             print("Soft AP started")
             print("MAC:"..wifi.ap.getmac().."\r\nIP:"..wifi.ap.getip());
             cnt = nil;cfg=nil;str=nil;ssidTemp=nil;
             collectgarbage()
			
			m = mqtt.Client("Coolpy-" .. node.chipid(), 120, "user", "pwd")
			m:on("message", function(conn, topic, data)
				if (data ~= nil) then
				  print (data)
				end
			end)
			m:on("offline", function(con) 
				m:close(); 
			end)
		
			m:connect(server,port,0,function(conn)
				print("Connected to:" .. server .. ":" .. port)
				mqtt_sub();
			end)
			function mqtt_sub()
				  ctopic = ukey..":".. hub..":"..cnode
					m:subscribe(ctopic, 0, function(conn)
					print("Subscribing topic: " .. ctopic)
				  end)
			end
			   
         end 
     end)
