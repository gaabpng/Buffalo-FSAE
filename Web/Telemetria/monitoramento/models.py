from django.db import models

# Create your models here.

class Lora(models.Model):
    timestamp = models.DateField(auto_now_add=True)
    RPM = models.IntegerField()
