#预测on状态结束所剩时间， 单位为秒
#每次记录一整个周期后更新平均时间，不同时间段有不同的平均时间
#结果可能为负数
from datetime import datetime, date

class UsageTimePredict:
    
    def __init__(self):
        self.startTime = datetime.now()
        self.endTime = datetime.now()
        self.averageUsageTime_morning = 600
        self.averageUsageTime_evening = 1000
        self.alpha = 0.9
        self.beta = 1.1
        self.preStatus = False

    def update(self, status):
        
        if status == True:
            if self.preStatus == True:
                result = self.statusStillOn()
            else:
                result = self.statusOn()
                
        else:
            if self.preStatus == True:
                self.status2Off()
                result = 0
            else:
                result = 0
        self.preStatus = status
        return result/60

    def statusOn(self):
        self.startTime = datetime.now()
        #self.startTime = self.startTime.strftime("%Y-%m-%d %H:%M:%S")
        total_seconds = 0
        if(self.endTime.hour<11 and self.endTime.hour>4):
            predict = self.averageUsageTime_morning-total_seconds
        else :
            predict = self.averageUsageTime_evening-total_seconds
        
        return predict

    def statusStillOn(self):
        self.endTime = datetime.now()
        #print(self.endTime)
        #self.endTime = self.endTime.strftime("%Y-%m-%d %H:%M:%S")
        total_seconds = (self.endTime - self.startTime).total_seconds()
        if(self.endTime.hour<11 and self.endTime.hour>4):
            predict = self.averageUsageTime_morning*self.beta-total_seconds
        else :
            predict = self.averageUsageTime_evening*self.beta-total_seconds
        
        return predict
    
    def status2Off(self):
        self.endTime = datetime.now()
        #print(self.endTime)
        #self.endTime = self.endTime.strftime("%Y-%m-%d %H:%M:%S")
        total_seconds = (self.endTime - self.startTime).total_seconds()
        if(self.endTime.hour<11 and self.endTime.hour>4):
            self.averageUsageTime_morning = self.alpha * self.averageUsageTime_morning + (1-self.alpha)*total_seconds
            #print(self.averageUsageTime_morning)
        else :
            self.averageUsageTime_evening = self.alpha * self.averageUsageTime_evening + (1-self.alpha)*total_seconds
            #print(self.averageUsageTime_evening)

    pass