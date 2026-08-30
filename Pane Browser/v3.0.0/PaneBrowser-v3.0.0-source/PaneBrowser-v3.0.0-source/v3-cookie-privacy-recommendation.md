# Analisis Cookies, Privasi, dan Kenyamanan Pane Browser v3.0.0

## Kesimpulan singkat

**Ada cara yang tepat dan relatif aman, dan sebagian besar mekanismenya sudah ada di v3.0.0.** Pane Browser dapat menyimpan cookies normal tanpa mengirim file History milik Pane Browser ke server Pane Browser dengan cara mempertahankan satu WebView2 User Data Folder (UDF) yang sama untuk sesi normal. Microsoft menjelaskan bahwa UDF WebView2 menyimpan cookies, permissions, cached resources, dan data browser lainnya [1].

Pada source v3.0.0, environment normal menggunakan folder:

```text
%LOCALAPPDATA%\\Pane Browser\\WebView2
```

Folder tersebut tidak dihapus ketika browser ditutup. Karena itu, cookie persisten dari website dapat tetap tersedia pada pembukaan berikutnya, selama website memang menetapkan cookie persisten, user-data folder tetap sama dan dapat ditulis, serta website tidak mencabut atau mengakhiri sesi tersebut.

## Hasil audit source

| Area | Temuan | Penilaian |
|---|---|---|
| Profile normal | Semua tab normal memakai environment dan UDF normal yang sama | Tepat untuk login/cookies yang bertahan |
| Cookie manager | Tidak diekspor atau dikirim oleh Pane Browser | Baik untuk privacy-by-default |
| Private tab | Menggunakan `IsInPrivateModeEnabled` melalui controller options; jika opsi gagal, tab private tidak dibuat sebagai tab normal | Lebih aman daripada fallback diam-diam |
| History aplikasi | Disimpan terpisah di `history.pbh` | Tidak sama dengan cookies WebView2 |
| Session restore | Hanya menulis URL HTTP(S) normal; private tab dikecualikan | Tepat |
| Clear browsing data | Dapat menghapus data profile jika API runtime tersedia | Perlu diuji pada Windows dengan beberapa versi runtime |
| Tracking prevention | Belum dikonfigurasi eksplisit oleh Pane Browser | Kandidat penyempurnaan kecil |
| Password/autofill | Belum diatur eksplisit oleh Pane Browser | Sebaiknya tidak diaktifkan atau diubah tanpa pilihan pengguna yang jelas |
| DNS/extension/Google account integration | Tidak diperlukan untuk tujuan ini dan tidak termasuk rekomendasi | Jangan ditambahkan pada penyempurnaan ini |

## Apakah cookies dapat tersimpan tanpa mengorbankan privacy?

**Ya.** Model yang paling seimbang adalah memisahkan dua hal berikut.

Pertama, profile normal menyimpan cookies dan site storage supaya website yang dipercaya tidak meminta login terus-menerus. Kedua, private tab memakai profile/controller private sehingga cookies normal tidak ikut digunakan dan tab tersebut tidak masuk ke History atau session restore. Dokumentasi Microsoft menjelaskan bahwa profile WebView2 memiliki CookieManager dan bahwa cookie berlaku bersama untuk WebView2 yang menggunakan profile tersebut [2]. Dokumentasi controller options juga mendefinisikan `IsInPrivateModeEnabled` sebagai opsi khusus untuk mengaktifkan InPrivate mode [3].

Dengan model ini, kenyamanan didapat dari profile normal yang persisten, sedangkan privasi didapat dari pilihan private tab ketika user tidak ingin meninggalkan data normal. **Jangan menghapus UDF normal setiap kali browser ditutup**, karena tindakan tersebut justru akan menghapus cookies dan login session yang ingin dipertahankan.

## Penyempurnaan kecil yang layak untuk v3.0.0

Jika nanti diizinkan untuk melakukan perubahan source, saya merekomendasikan hanya penyempurnaan berikut, tanpa extension, DNS, sinkronisasi cloud, atau integrasi login Google:

1. Tambahkan bagian Privacy & site data di Settings yang menjelaskan bahwa normal cookies/site storage dipertahankan secara lokal, sedangkan private tab terisolasi.
2. Tambahkan tombol untuk membersihkan **History saja**, **cookies/site data saja**, atau **semua data profile**. Tombol semua data harus diberi konfirmasi karena dapat mengeluarkan user dari website.
3. Atur tracking prevention WebView2 pada tingkat konservatif yang tetap menjaga kompatibilitas login. Microsoft mendokumentasikan bahwa level tracking prevention profile dapat disimpan di UDF dan berlaku untuk WebView2 yang memakai profile tersebut [2].
4. Tambahkan indikator lokasi profile normal dan status writable/readable, sehingga jika `%LOCALAPPDATA%` bermasalah user mendapat pesan yang jelas daripada mengira cookie hilang.
5. Tambahkan pilihan sederhana seperti `Keep me signed in on normal tabs` dan `Clear site data when closing private tabs`, tetapi jangan menerapkan penghapusan cookies normal secara otomatis.
6. Pastikan saat Clear browsing data dijalankan, profile normal tetap hidup dan operasi asynchronous selesai sebelum UI menyatakan berhasil.

Perubahan tersebut bersifat **penyempurnaan**, bukan perubahan arsitektur besar. Bila tidak ingin menambah risiko regression, v3.0.0 yang sekarang sebenarnya sudah memiliki fondasi yang benar untuk cookies normal persisten.

## Risiko dan batasan yang harus dipahami

Cookie tidak selalu berarti login akan selalu bertahan. Website dapat menetapkan session cookie yang memang hilang ketika sesi berakhir, memaksa login ulang karena kebijakan keamanan, menghapus cookie dari server, meminta verifikasi ulang, atau mengikat sesi ke perangkat/IP. WebView2 juga menyimpan lebih dari cookies: UDF dapat berisi permissions, cache, DOM storage, IndexedDB, dan site settings [1].

Privasi juga tidak berarti semua lalu lintas web berhenti keluar dari komputer. Pane Browser tidak mengirim `history.pbh` ke server Pane Browser, tetapi website yang dibuka menerima request normal, dan WebView2 mempunyai mekanisme diagnostic data/SmartScreen yang merupakan lapisan terpisah dari kode History Pane Browser [4]. Karena itu, dokumentasi aplikasi sebaiknya memakai kalimat **“Pane Browser does not upload its own browsing history to a Pane Browser server”**, bukan klaim absolut bahwa tidak ada data jaringan sama sekali.

Folder UDF harus writable dan sebaiknya tidak berada di network drive. Microsoft memperingatkan bahwa lokasi yang tidak writable dapat menyebabkan kegagalan startup atau kehilangan kemampuan menulis data [1]. Source v3 sudah menggunakan lokasi per-user di Local AppData, yang merupakan pilihan lebih tepat daripada folder instalasi yang terlindungi.

## Putusan

**Rekomendasi saya: jangan tambahkan extension, DNS, login Google terintegrasi, atau fitur besar lainnya.** Untuk kebutuhan yang kamu jelaskan, pendekatan terbaik adalah mempertahankan cookies normal secara lokal, menjaga private tab tetap terisolasi, dan bila diperlukan menambahkan Settings privacy yang memberi kontrol clear-data serta penjelasan yang jujur.

Source v3.0.0 saat ini **tidak perlu diubah hanya agar cookies dapat tersimpan**, karena penggunaan UDF normal yang sama sudah menyediakan mekanisme persistence tersebut. Perubahan tambahan sebaiknya baru dilakukan jika setelah pengujian nyata di Windows ditemukan bahwa cookies hilang, profile folder tidak writable, atau Clear browsing data tidak bekerja sesuai harapan.

## References

[1]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/user-data-folder "Microsoft Learn — Manage user data folders"
[2]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/winrt/microsoft_web_webview2_core/corewebview2profile "Microsoft Learn — CoreWebView2Profile Class"
[3]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2controlleroptions "Microsoft Learn — ICoreWebView2ControllerOptions"
[4]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/data-privacy "Microsoft Learn — Data and privacy in WebView2"
