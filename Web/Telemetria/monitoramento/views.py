from django.shortcuts import render
from rest_framework import generics
from .models import Lora
from .serializers import LoraSerializer

class LoraCreateView(generics.CreateAPIView):
    queryset = Lora.objects.all()
    serializer_class = LoraSerializer

class LoraListView(generics.ListAPIView):
    queryset = Lora.objects.order_by('-timestamp')[:50]
    serializer_class = LoraSerializer

def dashboard(request):
    return render(request, 'monitoramento/dashboard.html')