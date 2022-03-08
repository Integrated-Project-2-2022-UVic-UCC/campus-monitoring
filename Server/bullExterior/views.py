from django.shortcuts import render
from django.http.response import JsonResponse
from rest_framework.parsers import JSONParser
from rest_framework import status

from bullExterior.models import BullExteriro
from bullExterior.serializers import BullExteriorSerializer
from rest_framework.decorators import api_view
# Create your views here.

@api_view(['GET','POST'])
def bullExterior(request):
    if request.method == 'GET':
        p = BullExteriro.objects.all()
        print(p)
        title = request.GET.get('timestamp', None)
        if title is not None:
            p = p.filter(title__icontains=title)
        bullExterior_serializer = BullExteriorSerializer(p, many=True)
        print('serializer')
        return JsonResponse(bullExterior_serializer.data, safe=False)
    elif request.method == 'POST':
        bullExterior_data = JSONParser().parse(request)
        bullExterior_serializer = BullExteriorSerializer(data=bullExterior_data)
        if bullExterior_serializer.is_valid():
            print('guai')
            bullExterior_serializer.save()
            return JsonResponse(bullExterior_serializer.data, safe=False)
        return JsonResponse(bullExterior_serializer.errors, status=status.HTTP_400_BAD_REQUEST)
