from rest_framework import serializers
from bullExterior.models import BullExteriro

class BullExteriorSerializer(serializers.ModelSerializer):
    class Meta:
        model = BullExteriro
        fields =('id',
                'timestamp',
                'temperatura',
                'humitat',
                'eCO2',
                'TVOC',
                'pressio')