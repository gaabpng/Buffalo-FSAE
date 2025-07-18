from django.shortcuts import render
from rest_framework.views import APIView
from rest_framework import generics
from rest_framework.response import Response
from .models import Lora
from .serializers import LoraSerializer

class LoraCreateView(generics.CreateAPIView):
    queryset = Lora.objects.all()
    serializer_class = LoraSerializer

class LoraListView(generics.ListAPIView):
    queryset = Lora.objects.order_by('-timestamp')[:50]
    serializer_class = LoraSerializer

class LoraLastView(APIView):
    def get(self, request):
        ultimo = Lora.objects.order_by('-timestamp', '-id').first()
        serializer = LoraSerializer(ultimo)
        return Response(serializer.data)
    
def dashboard(request):
    return render(request, 'monitoramento/dashboard.html')