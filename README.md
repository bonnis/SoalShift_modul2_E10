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
Karena diminta untuk membuat program yang "otomatis" maka langkah pertama adalah membuat daemon dengan melakukan fork lalu me-kill parent process. Program lalu menjalankan perintah-perintah untuk menyelesaikan persoalan diatas, program kami sendiri melakukan hal ini 15 detik sekali. 

Langkah kedua adalah dengan mengecek apakah direktori /home/\[user]/modul2/gambar ada atau tidak dengan menggunakan opendir().
```c
DIR* dir = opendir("/home/bonnis/modul2/gambar");
```
Variabel dir tidak akan bernilai *NULL* jika foldernya telah dibuat. Jika folder belum dibuat, maka akan dibuatkan folder dengan menggunakan mkdir, jika tidak perlu maka proses tersebut dilewati. langkah-langkah berikutnya akan dilakukan di dalam loop daemon.

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
Langkah pertama untuk meyelesaikann soal ini adalah dengan melakukan ekstraksi terhadap file "campur2.zip" hal ini dilakukan dengan melakukan fork lalu membuat child process menmanggil exec untuk menjalakan program unzip.
```c
...
child_id = fork();
if(child_id == 0)
{
    char *argv[3]={"unzip","campur2.zip",NULL};
    execv("/usr/bin/unzip",argv);
}
...
```

Setelah ekstraksi dilakukan, maka akan dibuat file daftar.txt untuk di isi nantinya dengan list file yang emiliki ekstensi .txt. Hal ini dilakukan sangat mirip seperti proses ekstraksi diatas.

```c
...
child_id = fork();
if(child_id==0)
{
    execlp("touch","touch","daftar.txt",NULL);
}
...
```
Sekarang, kita butuh list file yang memiliki ekstensi .txt. Tentunya hal ini dilakukan dengan bantuan program ls, tetapi untuk mendapatkan output dari ls dari proses ls ke proses yang sekarang akan dibutuhkan pembuatan pipe. Untuk membuat pipe yang mengalir dari child ke parent, maka :
```c
...
int fd[2];
pipe(fd);
child_id=fork();
if(child_id==0)
{
    close(fd[0]);
    dup2(fd[1],1);
    ...
}
else
{
    close(fd[1]);
    ...
}
```
proses diatas akan membuat sebuah pipe satu arah yang mengalirkan data dari standard output dari child ke parent. Child lalu akan mengeksekusi program ls yang akan outputnya akan di proses oleh parent.

Untuk memudahkan proses pembacaan dari pipe, pipe pertama-tama dibuatkan dulu streamnya seperti ini :
```c
FILE* pipa=fdopen(fd[0],"r");
```
setelah itu maka bisa dilakukan *fgets()* ke stream pipa :
```c
while(fgets(buffer,sizeof(buffer),pipa)!=NULL)
```
loop diatas akan berhenti saat sudah tidak ada string yang bisa dibaca lagi dari stream pipa.

Setelah itu dideteksi file yang memiliki ekstensi .txt dengan menggunakan *strcmp()*, *strlen()* dan manipulasi string. Jika file tersebut benar memiliki ekstensi, maka akan tulis nama file tersebut di file daftar.txt yang telah dibuat.
```c
if(strcmp(&buffer[strlen(buffer)-4],".txt")==0)
{
    fprintf(berkas,"%s",buffer);
}
```
Variabel berkas diatas adalah stream yang menunjuk ke file "daftar.txt".

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

Karena program ini akan berjalan terus menerus, maka program ini akan dibuat sebagai Daemon. Cara pembuatan Daemon sama persis seperti nomor 1. Langkah pertama yang diakukan setelah membuat Daemon adalah membuat folder pertama. Yang menjadi masalah disini tentunya adalah penamaan foldernya yang harus sesuai dengan format dd:MM:yyy-hh:mm. Untuk memecahkan masalah ini maka digunakan :
```c
time_t t = time(NULL);
struct tm tm = *localtime(&t);
```
variabel tm berisi atribut-atribut yang kita butuhkan untuk menghasilkan format tersebut. Lalu kita buat formatnya dengan menggunakan fungsi *snprintf()* untuk memasukkan format tersebut ke dalam array of char(string).
```c
snprintf(wkt, sizeof wkt, "%02d:%02d:%04d-%02d:%02d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
```
Setelah mendapatkan nama folder tersebut, append kan namanya ke direktori log(/home/\[user]/log).
```c
strcat(dir,wkt);
```
Setelah itu, langsung saja jadikan variabel dir tersebut menjadi sebuah argumen dari mkdir dan jalankan mkdir menggunakan fork. Setelah dibuat folder awal nya untuk pertama kali, maka  akan masuk ke loop daemonnya.

Di program ini, loop Daemonnya akan mengulang tiap 60 detik, dibuat juga sebuah variabel i untuk menghitung menit yang di increment setiap satu loop selesai. Di setiap loop, akan dibuat log#i.log dengan mengcopy syslog ke dalam folder yang telah dibuat
```c
if(fork()==0)
{
    char log[15];
    snprintf(log, sizeof log, "/log%d.log", i);
    strcat(dir,log);
    execlp("cp", "cp", "/var/log/syslog", dir, NULL);
}
```
Tetapi, hal ini belum membuat folder baru tiap 30 menit, maka diatas dari kode diatas diberi sebuah persyaratan jika variabel i>=31 dia akan membuat sebuah folder baru, caranya mirip mirip dengan pembuatan folder pertama.
```c
if(i>=31 || !rek)
{
    i=1;
    t=time(NULL);
    tm=*localtime(&t);
    memset(dir,0,sizeof(dir));
    strcpy(dir,"/home/bonnis/log/");
    snprintf(wkt, sizeof wkt, "%02d:%02d:%04d-%02d:%02d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
    strcat(dir,wkt);
    if(fork()==0)
    {
        execlp("mkdir","mkdir","-p",dir,NULL);
    }
    else
    {
        while(wait(&status)>0);
    }
}
```
Variabel *rek* diatas adalah sebuah DIR* yang berfungsi untuk mengecek ada apa tidaknya folder yang lama, jika folder yang lama hilang, maka akan dibuat folder baru.

Untuk poin b, awalnya saya mengerjakan dengan membuat program pendek melakukan exec pkill. Tetapi karena ada kemungkinan nama programnya di rename, terpikir untuk mengkill programnya langsung dari PID nya. Cara yang saya dapat adalah dengan membuat proses utama membuat sebuah program untuk mengkill dirinya sendiri. Program yang dibuat akan melakukan kill terhadap pid proses utama, pid ini didapat dari proses utama itu sendiri.
```c
FILE* stop = fopen("Soal5b.c","w");
fprintf(stop,"#include <unistd.h>\n#include <sys/wait.h>\nint main(){int dump;if(fork()==0)execlp(\"kill\",\"kill\",\"%d\",NULL);else{while(wait(&dump)>0);}execlp(\"rm\",\"rm\",\"kill5b.out\",NULL);}",getpid());
fclose(stop);
if(fork()==0)
{
    execlp("gcc", "gcc", "Soal5b.c", "-o", "kill5b.out", NULL);
}
```
Karena pid yang sudah di kill bisa dipakai oleh proses lain, untuk menghindari salah mengkill karena programnya secara tidak sengaja di eksekusi, program C yang mengkill otomatis mendelete dirinya sendiri.


---

