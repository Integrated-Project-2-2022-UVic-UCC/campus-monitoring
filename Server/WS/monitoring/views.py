from django.shortcuts import render
from django.http.response import JsonResponse
from rest_framework.parsers import JSONParser
from rest_framework import status

from monitoring.models import Monitoring
from monitoring.serializers import MonitoringSerializer
from rest_framework.decorators import api_view

# Create your views here.

@api_view(['POST'])
def monitoring_data(request):
    maquina_data = JSONParser().parse(request)
    monitoring_serializer = MonitoringSerializer(data=maquina_data)
    if monitoring_serializer.is_valid():
        print('guai')
        monitoring_serializer.save()
        return JsonResponse(monitoring_serializer.data, safe=False)
    return JsonResponse(monitoring_serializer.errors, status=status.HTTP_400_BAD_REQUEST)

# @api_view(['POST'])
# def maquina_list(request):
#     maquina_data = JSONParser().parse(request)
#     productividad_serializer = ProductividadSerializer(data=maquina_data)
#     if productividad_serializer.is_valid():
#         print('guai')
#         productividad_serializer.save()
#         return JsonResponse(productividad_serializer.data, safe=False)
#     return JsonResponse(productividad_serializer.errors, status=status.HTTP_400_BAD_REQUEST)
