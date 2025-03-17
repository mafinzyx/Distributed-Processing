import threading
import queue
import time
import random

class BinarySemaphore:
    def __init__(self):
        self.channel = queue.Queue(maxsize=1)
        self.channel.put(1)  # Początkowy stan odblokowany (1)

    def acquire(self):
        """Funkcja blokująca dostęp do zasobu"""
        self.channel.get()  # Pobierz 1 i zablokuj

    def release(self):
        """Funkcja odblokowująca dostęp do zasobu"""
        self.channel.put(1)  # Przywróć 1, odblokuj

# Wspólny zasób
shared_resource = []

# Semafor binarny
semaphore = BinarySemaphore()

def producer():
    """Wątek producenta"""
    for i in range(5):
        semaphore.acquire()  # Próba dostępu do zasobu
        item = random.randint(1, 100)
        shared_resource.append(item)
        print(f"[Producer] Produced item: {item}")
        time.sleep(random.uniform(0.5, 1.5))  # Symulacja pracy
        semaphore.release()  # Zwolnienie semafora
        time.sleep(random.uniform(0.5, 1.5))

def consumer():
    """Wątek konsumenta"""
    for _ in range(5):
        semaphore.acquire()  # Próba dostępu do zasobu
        if shared_resource:
            item = shared_resource.pop(0)
            print(f"[Consumer] Consumed item: {item}")
        else:
            print("[Consumer] No item to consume.")
        time.sleep(random.uniform(0.5, 1.5))  # Symulacja pracy
        semaphore.release()  # Zwolnienie semafora
        time.sleep(random.uniform(0.5, 1.5))

# Utworzenie wątków producenta i konsumenta
producer_thread = threading.Thread(target=producer)
consumer_thread = threading.Thread(target=consumer)

# Uruchomienie wątków
producer_thread.start()
consumer_thread.start()

# Oczekiwanie na zakończenie wątków
producer_thread.join()
consumer_thread.join()

print("Program zakończył działanie.")
