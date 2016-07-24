	print("Coolpy5 V1.0 ESP8266_SDK HTTP POST")
	server = "192.168.1.105"--服务器域名或IP地址
	port = 6543--酷痞平台服务端口
	ukey = "51dd40af-135f-4733-89ff-a07615282812"--UserKey用户密钥(必改项)
	hub = 5--Hub ID(必改项)
	cnode = 8--Node ID(必改项)
	--(让芯片连接互联网)
	wifiPoint = "YMS_805_1"--本地路由器热点名
	wifiPwd = "yms*****"--本地路由器密码
    
	wifiok = 0;
	tmr.stop(0);
	tmr.stop(1);
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
			 wifiok =1;
             print("Soft AP started")
             print("MAC:"..wifi.ap.getmac().."\r\nIP:"..wifi.ap.getip());
             cnt = nil;cfg=nil;str=nil;ssidTemp=nil;
             collectgarbage()
         end 
     end)
	
	--每隔10秒向服务器提交一个数据
	tmr.alarm(1, 10000, 1, function() 
		if(wifiok == 1) then --判断wifi是否已经成功连接主路由器连接互联网
			sk=net.createConnection(net.TCP, 0)
			sk:on("connection", function(conn) topost() end)
			sk:on("disconnection", function(conn, pl) print("disconnection") sk:close() end)
			sk:on("receive", function(conn, pl) sk:close() print(pl) end ) 
			sk:connect(port,server)
		end
		
		function topost()
			val="{\"value\":100}"--定义要提交的数据json格式
			
			sk:send("POST /api/hub/"..hub.."/node/"..cnode.."/datapoints HTTP/1.1\r\n"
			.."Host: "..server.."\r\n"
			.."Content-Length: "..string.len(val).."\r\n"
			.."U-ApiKey:"..ukey.."\r\n"
			.."Cache-Control: no-cache\r\n\r\n"
			..val.."\r\n" )
		end
	end)	
