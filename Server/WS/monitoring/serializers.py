from rest_framework import serializers
from monitoring.models import Monitoring

class MonitoringSerializer(serializers.ModelSerializer):
    class Meta:
        model = Monitoring
        fields = ('id',
                  'timestamp',
                  'sensorID',
                  'var1',
                  'var2',
                  'var3',
                  'var4',
                  'var5',
                  'var6',
                  'var7'
                  )