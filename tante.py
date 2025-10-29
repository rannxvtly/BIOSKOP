import time

def sing_song():
    lyrics = [
        "tanteeee...",
        "sudah terbiasa terjadi tante",
        "teman datang ketika lagi butuh saja",
        "coba kalau lagi susah",
        "mereka semua menghilaaaang.............."
    ]

    for line in lyrics:
        print(line)
        time.sleep(3.0)

if __name__ == "__main__":
    sing_song()