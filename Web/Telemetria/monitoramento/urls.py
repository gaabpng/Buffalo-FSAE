from django.urls import path
from .views import LoraCreateView, LoraListView, dashboard

urlpatterns = [
    path('dados/', LoraCreateView.as_view(), name='criar-lora'),
    path('dados/lista/', LoraListView.as_view(), name='listar_lora'),
    path('dashboard/', dashboard, name='dashboard')
]