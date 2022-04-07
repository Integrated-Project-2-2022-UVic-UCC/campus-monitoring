from django.urls import include, re_path
from monitoring import views

urlpatterns = [
    re_path(r'^api/monitoring$', views.monitoring_data)
]