# Laporan Soal Shift Modul 2
Laporan pengerjaan soal shift modul kedua
Kelas Sistem Operasi E Jurusan Informatika Institut Teknologi Sepuluh Nopember  
Oleh Kelompok E10

---

## Nomor 1
### Soal
Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “\[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/\[user]/modul2/gambar.  
Catatan : Tidak boleh menggunakan crontab.
### Jawab
Karena diminta untuk membuat program yang "otomatis" maka langkah pertama adalah membuat daemon dengan melakukan fork lalu me-kill parent process. Program lalu menjalankan perintah-perintah untuk menyelesaikan persoalan diatas, program kami sendiri melakukan hal ini 15 detik sekali. langkah-langkah berikutnya akan dilakukan di dalam loop daemon.

Langkah kedua adalah dengan mengecek apakah direktori /home/\[user]/modul2/gambar ada atau tidak dengan menggunakan opendir().
```c
DIR* dir = opendir("/home/bonnis/modul2/gambar");
```
Variabel dir tidak akan bernilai *NULL* jika foldernya telah dibuat. Jika folder belum dibuat, maka akan dibuatkan folder dengan menggunakan mkdir, jika tidak perlu maka proses tersebut dilewati.

Langkah ketiga adalah mencari setiap file yang berformat *.png* lalu merenamenya dengan tambahan *_grey.png* di belakangnya. hal pertama yang dilakukan adalah mendapatkan nama tiap file di folder yang ada, hal ini dilakukan dengan menggunakan fungsi *readdir()*.
```c
while((ent = readdir(dir)) != NULL) {
    ...
    strcpy(old,ent->d_name);
    ...
}
```
dimana ent adalah variabel bertipe data *struct dirent* yang merepresentasikan nama file dan direktori yang ada di dalam folder tersebut.

Lalu dilakukan pengecekan apakah file saat ini berformat *.png* atau bukan hal ini dilakukan dengan menggunakan fungsi *strcmp*, *strlen* dan manipulasi array
```c
 if (strcmp(&old[strlen(old)-4],".png")==0) 
```
jika file tersebut tidak berformat .png maka proses langsung dilanjutkan ke file lain, jika file tersebut memang benar berformat *.png* maka akan dilakukan rename dan pemindahan.

Rename dilakukan dilakukan dengan menggunakan fungsi *rename()*.
```C
strncpy(new,old,(strlen(old)-4));
strcat(new,"_grey.png");
rename(old,new);
```
Setelah dilakukan rename, maka dilakukan pemindahan dengan pertama-tama melakukan fork, lalu membuat child process melakukan pemanggilan exec untuk menjalankan program mv dengan argumen yang sesuai.

```c
...
child_id=fork();
if (child_id == 0) 
{
    char *argv[4] = {"mv", new, "/home/bonnis/modul2/gambar", NULL};
    execv("/bin/mv", argv);
} 
...
```
Daemon ini akan otomatis melakukan setiap operasi diatas setiap 15 detik sekali.

---

## Nomor 2
### Soal
Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku”pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab
### Jawab

---

## Nomor 3
### Soal
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
 1. mengekstrak file zip tersebut.
 2. menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt.   

Catatan:  
* Gunakan fork dan exec.
* Gunakan minimal 3 proses yang diakhiri dengan exec.
* Gunakan pipe
Pastikan file daftar.txt dapat diakses dari text editor

### Jawab

---

## Nomor 4
### Soal  
Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).  
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.  

Contoh:
File makan_enak.txt terakhir dibuka pada detik ke-1
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt  

Catatan: 
* dilarang menggunakan crontab
* Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst


### Jawab

---

## Nomor 5
### Soal
Kerjakan poin a dan b di bawah:  
<ol type="a">
<li>Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log  

Ket:
Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
Per menit memasukkan log#.log ke dalam folder tersebut  

‘#’ : increment per menit. Mulai dari 1</li>
<li> Buatlah program c untuk menghentikan program di atas.
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.</li>
</ol>

### Jawab

---

