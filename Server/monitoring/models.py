from django.db import models

# Create your models here.
class Monitoring(models.Model):
    timestamp = models.DateTimeField(default='')
    sensorID = models.IntegerField(default='')
    var1 = models.DecimalField(max_digits=6, decimal_places=2)
    var2 = models.DecimalField(max_digits=6, decimal_places=2)
    var3 = models.DecimalField(max_digits=6, decimal_places=2)
    var4 = models.DecimalField(max_digits=6, decimal_places=2)
    var5 = models.DecimalField(max_digits=6, decimal_places=2)
    var6 = models.DecimalField(max_digits=6, decimal_places=2)
    var7 = models.DecimalField(max_digits=6, decimal_places=2)
