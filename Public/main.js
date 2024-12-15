// Menghubungkan ke WebSocket server
const divPesan = document.getElementById('pesan');
const inputPesan = document.getElementById('inputPesan');

const ws = new WebSocket('ws://localhost:9091');

let namaPengguna = ''; // Menyimpan nama pengguna lokal

ws.onopen = () => {
    tambahkanPesan('Terhubung ke server.');
    namaPengguna = prompt("Masukkan nama Anda:");
    ws.send(namaPengguna); // Mengirimkan nama sebagai pesan pertama
};

ws.onmessage = (event) => {
    const pesan = event.data;

    // Pisahkan nama pengirim dan isi pesan berdasarkan ": "
    const [namaPengirim, ...isiPesanArr] = pesan.split(': ');
    const isiPesan = isiPesanArr.join(': '); // Gabungkan kembali isi pesan

    // Jika pesan dari pengirim (diri sendiri), ganti nama menjadi "Kamu"
    if (namaPengirim === namaPengguna) {
        tambahkanPesan(`Kamu: ${isiPesan}`);
    } else {
        tambahkanPesan(pesan); // Pesan dari pengguna lain ditampilkan apa adanya
    }
};

ws.onclose = () => {
    tambahkanPesan('Koneksi ke server terputus.');
};

function kirimPesan() {
    const pesan = inputPesan.value.trim();
    if (pesan) {
        ws.send(pesan); // Mengirim pesan ke server
        inputPesan.value = ''; // Mengosongkan input setelah pesan dikirim
        if (pesan.toLowerCase() === 'exit') {
            ws.close(); // Menutup koneksi jika pengguna mengetik "exit"
        }
    }
}

function tambahkanPesan(pesan) {
    const elemenBaru = document.createElement('div');
    elemenBaru.textContent = pesan; // Menambahkan pesan ke elemen div
    divPesan.appendChild(elemenBaru);
    divPesan.scrollTop = divPesan.scrollHeight; // Scroll otomatis ke bawah
}