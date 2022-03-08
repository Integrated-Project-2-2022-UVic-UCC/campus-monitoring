from django.db import models

# Create your models here.
class BullExteriro(models.Model):
    timestamp = models.DateTimeField(default='')
    temperatura =models.DecimalField( max_digits=3, decimal_places=1)
    humitat =models.DecimalField(max_digits=3, decimal_places=1)
    eCO2 = models.IntegerField(default='')
    TVOC = models.IntegerField(default='')
    pressio = models.IntegerField(default='')

