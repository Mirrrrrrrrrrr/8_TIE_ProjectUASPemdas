#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Task {
    string nama;
    bool selesai;
};

Task listTugas[50];
int jumlah = 0;

// ---- Rekursif: hitung jumlah tugas selesai ----
int hitungSelesai(int i) {
    if (i == jumlah) return 0;
    return (listTugas[i].selesai ? 1 : 0) + hitungSelesai(i + 1);
}

// ---- Pointer: ubah status tugas ----
void tandaiSelesai(Task* t) {
    t->selesai = true;
}

// ---- Simpan ke file ----
void simpanFile() {
    ofstream file("catatan.txt");
    for (int i = 0; i < jumlah; i++) {
        string statusTeks = listTugas[i].selesai ? "Selesai" : "Belum";
        file << listTugas[i].nama << "|" << statusTeks << endl;
    }
    file.close();
    cout << "Data disimpan.\n";
}

// ---- Memuat file ----
void muatFile() {
    ifstream file("catatan.txt");
    if (!file.is_open()) return;
    jumlah = 0;
    string line;

    while (getline(file, line)) {
        int pos = line.find('|');
        if (pos == -1) continue;
        listTugas[jumlah].nama = line.substr(0, pos);
        string statusTeks = line.substr(pos + 1);

        if (statusTeks == "Selesai") {
            listTugas[jumlah].selesai = true;
        } else {
            listTugas[jumlah].selesai = false;
        }
        jumlah++;
    }
    file.close();
}

// ---- Tambah tugas ----
void tambahTugas() {
    if (jumlah >= 50) {
        cout << "List penuh!\n";
        return;
    }

    cout << "Nama tugas: ";
    cin.ignore();
    getline(cin, listTugas[jumlah].nama);

    listTugas[jumlah].selesai = false;
    jumlah++;
    simpanFile();
    cout << "Tugas ditambahkan!\n";
}

// ---- Hapus tugas ----
void hapusTugas(int index) {
    for (int i = index; i < jumlah - 1; i++) {
        listTugas[i] = listTugas[i + 1];
    }
    cout << "Tugas berhasil dihapus dari memori.\n";
    jumlah--;
    simpanFile();
}

// ---- Tampilkan tugas ----
void tampilTugas() {
    if (jumlah == 0) {
        cout << "Belum ada tugas.\n";
        return;
    }

    cout << "\n=== DAFTAR TUGAS ===\n";
    for (int i = 0; i < jumlah; i++) {
        cout << i + 1 << ". " << listTugas[i].nama
            << " [" << (listTugas[i].selesai ? "Selesai" : "Belum") << "]\n";
    }
}


// ---- Hapus tugas untuk menu ----
void menuHapus() {
    if (jumlah == 0) {
        cout << "Tidak ada tugas untuk dihapus.\n";
        return;
    }
    tampilTugas();

    int pilih;
    cout << "Pilih nomor tugas yang akan DIHAPUS: ";
    cin >> pilih;
    if (pilih < 1 || pilih > jumlah) {
        cout << "Nomor tidak valid!\n";
        return;
    }

    char yakin;
    cout << "Yakin hapus '" << listTugas[pilih - 1].nama << "'? (y/n): ";
    cin >> yakin;
    if (yakin == 'y' || yakin == 'Y') {
        hapusTugas(pilih - 1);
    } else {
        cout << "Penghapusan dibatalkan.\n";
    }
}

// ---- Menu tandai selesai ----
void menuSelesai() {
    tampilTugas();
    int pilih;
    cout << "Pilih nomor tugas: ";
    cin >> pilih;

    if (pilih < 1 || pilih > jumlah) {
        cout << "Nomor tidak valid!\n";
        return;
    }

    tandaiSelesai(&listTugas[pilih - 1]);
    simpanFile();
    cout << "Tugas ditandai selesai.\n";
}

// ---- Menu utama ----
void menu() {
    int pilih;
    do {
        cout << "\n=== TO-DO LIST ===\n";
        cout << "1. Tambah Tugas\n";
        cout << "2. Tampilkan Tugas\n";
        cout << "3. Tandai Selesai (Pointer)\n";
        cout << "4. Hitung Tugas Selesai (Rekursif)\n";
        cout << "5. Hapus Tugas\n";
        cout << "6. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilih;

        switch (pilih) {
            case 1: tambahTugas(); break;
            case 2: tampilTugas(); break;
            case 3: menuSelesai(); break;
            case 4:
                cout << "Tugas selesai: " << hitungSelesai(0) << endl;
                break;
            case 5: menuHapus(); break;
            case 6: cout << "Keluar...\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilih != 6);
}

int main() {
    muatFile();
    menu();
    return 0;
}
