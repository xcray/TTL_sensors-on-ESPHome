#include "esphome.h"

class TTLSensor : public Component, public uart::UARTDevice {
    public:
      TTLSensor(UARTComponent *parent) : UARTDevice(parent) {}
      Sensor *door_status = new Sensor();
      Sensor *lock_event = new Sensor();
      Sensor *levt_keyid = new Sensor();
      Sensor *levt_time = new Sensor();
      Sensor *door_event = new Sensor();
      Sensor *door_evttime = new Sensor();
      Sensor *lock_battery = new Sensor();
      Sensor *lock_batttime = new Sensor();
      Sensor *bt_temp = new Sensor();
      Sensor *bt_humi = new Sensor();
      Sensor *bt_batt = new Sensor();
      Sensor *flora_temp_a = new Sensor();
      Sensor *flora_temp_b = new Sensor();
      Sensor *flora_mois_a = new Sensor();
      Sensor *flora_mois_b = new Sensor();
      Sensor *flora_lumi_a = new Sensor();
      Sensor *flora_lumi_b = new Sensor();
      Sensor *flora_soil_a = new Sensor();
      Sensor *flora_soil_b = new Sensor();
      
      void setup() override {
    // nothing to do here
  }
String revstr(String rawstr) {
  if (rawstr.length() % 2 == 1) {
    ESP_LOGD("TTLSensor", "wrong length of hexstr");
    rawstr += "0";
    }
  String result = "";
  for (int i=0;i<=rawstr.length()/2;i++)
    result = rawstr.substring(i*2,i*2+2) + result;
  return result;
  }
  
  void loop() override {
    // Use Arduino API to read data, for example
    while (available()){
    String line = readStringUntil('\n');
    int index = line.indexOf("async.ble_event"); 
    if ( index >0) {
      String result = line.substring(index+26,line.length()-1);//获得json串
      //处理数据，首先提取pdid,did,eid,edata
      String did = result.substring(15,result.indexOf("mac")-3);
      String pdid = result.substring(result.indexOf("pdid")+6,result.indexOf("evt")-3);
      String eid = result.substring(result.indexOf("eid")+5,result.indexOf("edata")-2);
      String edata = result.substring(result.indexOf("edata")+8,result.indexOf("frmCnt")-5);
      String topic = String(pdid)+"/"+did+"/"+eid+"/"+edata;

    if (pdid == "407") //榉树门锁
    {
        if (eid == "4110") //状态报告
          door_status->publish_state(strtoul(edata.c_str(),NULL,16));
        else if (eid == "5") //锁事件
        {
            lock_event->publish_state(strtoul(edata.substring(0,4).c_str(),NULL,16));
            levt_keyid->publish_state(strtoul(revstr(edata.substring(4,12)).c_str(),NULL,16));
            levt_time->publish_state(strtoul(revstr(edata.substring(12,20)).c_str(),NULL,16));
        }
        else if (eid == "7") //门事件
        {
            door_event->publish_state(strtoul(edata.substring(0,2).c_str(),NULL,16));
            door_evttime->publish_state(strtoul(revstr(edata.substring(2,10)).c_str(),NULL,16));
        }
        else if (eid == "4106") //电量报告
        {
            lock_battery->publish_state(strtoul(edata.substring(0,2).c_str(),NULL,16));
            lock_batttime->publish_state(strtoul(revstr(edata.substring(2,10)).c_str(),NULL,16));
        }
        else ESP_LOGD("TTLSensor", "Unknown eid of lock");
    }
    else if (pdid == "1371") //蓝牙温湿度2
    {
        if (eid == "4100") //温度，逆序已调整，进制转换
          bt_temp->publish_state(strtoul(revstr(edata).c_str(),NULL,16)*0.1);
        else if (eid == "4102") //湿度
          bt_humi->publish_state(strtoul(revstr(edata).c_str(),NULL,16)*0.1);
        else if (eid == "4106") //电量
          bt_batt->publish_state(strtoul(edata.c_str(),NULL,16));
        else ESP_LOGD("TTLSensor", "Unknown eid of BT TMHM 2");
    }
    else if (pdid == "152") //花花草草
    {
        if (did == "xxxxxx") //第一个，南瓜 Should be modified.
        {
          if (eid == "4100") //温度，逆序已调整，进制转换
            flora_temp_a->publish_state(strtoul(revstr(edata).c_str(),NULL,16)*0.1);
          else if (eid == "4103") //照度
            flora_lumi_a->publish_state(strtoul(revstr(edata).c_str(),NULL,16));
          else if (eid == "4104") //湿度
            flora_mois_a->publish_state(strtoul(edata.c_str(),NULL,16));
          else if (eid == "4105") //肥力
            flora_soil_a->publish_state(strtoul(revstr(edata).c_str(),NULL,16));
          else ESP_LOGD("TTLSensor", "Unknown eid of Flora A");
        }
        else //第二个，绿萝，只有两个，不再做额外判断
        {
          if (eid == "4100") //温度，逆序已调整，进制转换
            flora_temp_b->publish_state(strtoul(revstr(edata).c_str(),NULL,16)*0.1);
          else if (eid == "4103") //照度
            flora_lumi_b->publish_state(strtoul(revstr(edata).c_str(),NULL,16));
          else if (eid == "4104") //湿度
            flora_mois_b->publish_state(strtoul(edata.c_str(),NULL,16));
          else if (eid == "4105") //肥力
            flora_soil_b->publish_state(strtoul(revstr(edata).c_str(),NULL,16));
          else ESP_LOGD("TTLSensor", "Unknown eid of Flora B");
        }
    }
    else ESP_LOGD("TTLSensor", "Unknown pdid");
    //ESP_LOGD("TTLSensor", topic.c_str());
    }
    } //end of while (available())
  } // end of loop()
};
