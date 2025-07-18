from rest_framework import serializers
from .models import Lora

class LoraSerializer(serializers.ModelSerializer):
    class Meta:
        model = Lora
        fields = '__all__'