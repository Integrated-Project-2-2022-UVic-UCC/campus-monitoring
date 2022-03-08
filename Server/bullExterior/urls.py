# from django.conf.urls import url
from bullExterior import views
from django.urls import re_path

urlpatterns = [
    re_path(r'^api/bullExterior$',views.bullExterior)
]