#include <iostream>
#include <iomanip>
#include <string>
#define max 10
using namespace std;

struct queue {
    int head;
    int tail;
    int no = 0;
    string data[max];
};

struct TransaksiBank {
    int kodeLayanan;
    string deskripsi;
    int nominal;
};

struct Layanan {
    string nama;
    string keterangan;
};

struct PelayananAntrian {
    string nomorAntrian;
    string status;
    string nama;
    string nomorRekening;
    string tanggal;
    string waktu;
} dataPelayananAntrian;

queue antri;
TransaksiBank transaksiBank[100];

Layanan daftarLayanan[4] = {
    {"Setor Tunai", "Menambahkan uang ke dalam rekening"},
    {"Tarik Tunai", "Mengambil uang dari rekening"},
    {"Buka Rekening", "Membuka rekening baru"},
    {"Cek Saldo", "Melihat saldo terakhir"}
};

void inisialisasi() {
    antri.head = -1;
    antri.tail = -1;
}

int isEmpty() {
    return antri.tail == -1;
}

int isFull() {
    return antri.tail == max - 1;
}

void Enqueue() {
    if (!isFull()) {
        antri.head = 0;
        antri.tail++;
        antri.no++;
        antri.data[antri.tail] = "B" + to_string(antri.no);
    } else {
        cout << "Antrian penuh\n";
    }
}

void dequeue() {
    if (!isEmpty()) {
        for (int i = antri.head; i < antri.tail; i++) {
            antri.data[i] = antri.data[i + 1];
        }
        antri.tail--;
    }
}

void showAntrian() {
    cout << "\nDaftar Antrian:\n";
    for (int i = 0; i <= antri.tail; i++) {
        cout << i + 1 << ". " << antri.data[i] << endl;
    }
    cout << endl;
}

void menuTransaksi() {
    int totalTransaksi = 0;
    int pilihan, nominal;
    char lanjut;

    cin.ignore();
    cout << "Masukkan Tanggal (dd-mm-yyyy): "; getline(cin, dataPelayananAntrian.tanggal);
    cout << "Masukkan Waktu (hh:mm): "; getline(cin, dataPelayananAntrian.waktu);
    cout << "Nama Nasabah : "; getline(cin, dataPelayananAntrian.nama);
    cout << "Nomor Rekening (kosongkan jika buka rekening baru): "; getline(cin, dataPelayananAntrian.nomorRekening);

    dataPelayananAntrian.nomorAntrian = antri.data[antri.head];

    do {
        system("cls");
        cout << "\nLayanan Tersedia:\n";
        for (int i = 0; i < 4; i++) {
            cout << i + 1 << ". " << daftarLayanan[i].nama << " - " << daftarLayanan[i].keterangan << endl;
        }
        cout << "\nPilih layanan (1-4): "; cin >> pilihan;

        while (pilihan < 1 || pilihan > 4) {
            cout << "Pilihan tidak valid. Masukkan 1-4: "; cin >> pilihan;
        }

        cin.ignore();
        cout << "Deskripsi (opsional): "; getline(cin, transaksiBank[totalTransaksi].deskripsi);

        if (pilihan == 1 || pilihan == 2) {
            cout << "Nominal (Rp): "; cin >> transaksiBank[totalTransaksi].nominal;
        } else {
            transaksiBank[totalTransaksi].nominal = 0;
        }

        transaksiBank[totalTransaksi].kodeLayanan = pilihan - 1;
        totalTransaksi++;

        cout << "\nTambah transaksi lagi? (y/n): "; cin >> lanjut;
    } while (lanjut == 'y' || lanjut == 'Y');

    system("cls");
    cout << "========================= STRUK TRANSAKSI =========================\n";
    cout << "Nomor Antrian : " << dataPelayananAntrian.nomorAntrian << endl;
    cout << "Nama          : " << dataPelayananAntrian.nama << endl;
    cout << "No Rekening   : " << (dataPelayananAntrian.nomorRekening.empty() ? "-" : dataPelayananAntrian.nomorRekening) << endl;
    cout << "Tanggal/Waktu : " << dataPelayananAntrian.tanggal << " / " << dataPelayananAntrian.waktu << endl;
    cout << "------------------------------------------------------------------\n";

    for (int i = 0; i < totalTransaksi; i++) {
        cout << i + 1 << ". " << daftarLayanan[transaksiBank[i].kodeLayanan].nama
             << " - Rp. " << transaksiBank[i].nominal
             << (transaksiBank[i].deskripsi.empty() ? "" : (" (" + transaksiBank[i].deskripsi + ")")) << endl;
    }

    cout << "==================================================================\n";
    cout << "         Terima kasih telah menggunakan layanan kami.             \n";
    cout << "==================================================================\n";
    system("pause");
}

int main() {
    int pil;
    inisialisasi();
    do {
        system("cls");
        cout << "==================== BANK QUEUE SYSTEM ====================\n";
        showAntrian();
        cout << "1. Ambil Nomor Antrian\n";
        cout << "2. Layani Nasabah\n";
        cout << "3. Hapus Semua Antrian\n";
        cout << "4. Keluar\n";
        cout << "Pilih menu: "; cin >> pil;

        switch (pil) {
        case 1:
            Enqueue();
            cout << "Nomor Antrian Anda: " << antri.data[antri.tail] << endl;
            break;
        case 2:
            if (!isEmpty()) {
                cout << "Memanggil antrian: " << antri.data[antri.head] << endl;
                system("pause");
                menuTransaksi();
                dequeue();
                dataPelayananAntrian = {};
            } else {
                cout << "Tidak ada antrian.\n";
            }
            break;
        case 3:
            antri = {};
            cout << "Antrian dikosongkan.\n";
            break;
        case 4:
            cout << "Terima kasih telah menggunakan sistem kami.\n";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
        }
        if (pil != 4) system("pause");
    } while (pil != 4);
    return 0;
}
