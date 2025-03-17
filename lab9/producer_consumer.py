import threading
import random
import time
from collections import defaultdict

class Magazyn:
    def __init__(self, max_capacity):
        self.max_capacity = max_capacity  # Maksymalna pojemność magazynu
        self.magazyn = defaultdict(int)  # Słownik przechowujący produkty i ich ilości
        self.lock = threading.Lock()  # Zamek (lock) do synchronizacji dostępu

    def produkcja(self, produkt, ilosc):
        """Dodaje produkt do magazynu"""
        with self.lock:  # Zapewnienie synchronizacji dostępu do magazynu
            if self.zajetosc() + ilosc <= self.max_capacity:  # Sprawdzamy, czy jest miejsce w magazynie
                self.magazyn[produkt] += ilosc
                print(f"Producenci dodali {ilosc} {produkt}(ów) do magazynu.")
            else:
                print("Brak miejsca w magazynie!")

    def konsumpcja(self, produkt, ilosc):
        """Usuwa produkt z magazynu, jeśli jest dostępny"""
        with self.lock:  # Zapewnienie synchronizacji dostępu do magazynu
            if self.magazyn[produkt] >= ilosc:
                self.magazyn[produkt] -= ilosc
                print(f"Konsumenci kupili {ilosc} {produkt}(ów) z magazynu.")
            else:
                print(f"Brak {produkt} w magazynie do zakupu!")

    def zajetosc(self):
        """Zwraca łączną ilość produktów w magazynie"""
        return sum(self.magazyn.values())

    def stan_magazynu(self):
        """Wypisuje aktualny stan magazynu"""
        with self.lock:
            print(f"Stan magazynu: {dict(self.magazyn)}")

def producent(magazyn):
    produkty = ["jabłka", "pomarańcze", "banany", "gruszki"]
    while True:
        produkt = random.choice(produkty)
        ilosc = random.randint(1, 10)
        magazyn.produkcja(produkt, ilosc)
        time.sleep(random.uniform(1, 3))  # Losowe opóźnienie

def konsument(magazyn):
    produkty = ["jabłka", "pomarańcze", "banany", "gruszki"]
    while True:
        produkt = random.choice(produkty)
        ilosc = random.randint(1, 5)
        magazyn.konsumpcja(produkt, ilosc)
        time.sleep(random.uniform(1, 3))  # Losowe opóźnienie

# Ustawienia
max_capacity = 50  # Maksymalna pojemność magazynu
magazyn = Magazyn(max_capacity)

# Tworzenie wątków producentów i konsumentów
producenci = [threading.Thread(target=producent, args=(magazyn,)) for _ in range(3)]
konsumenci = [threading.Thread(target=konsument, args=(magazyn,)) for _ in range(3)]

# Uruchamianie wątków
for p in producenci:
    p.start()

for k in konsumenci:
    k.start()

# Program będzie działał w nieskończoność