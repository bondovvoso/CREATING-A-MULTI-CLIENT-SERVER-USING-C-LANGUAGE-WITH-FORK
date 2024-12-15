// Menghubungkan ke WebSocket server
const divPesan = document.getElementById('pesan');
const inputPesan = document.getElementById('inputPesan');

const ws = new WebSocket('ws://localhost:9091');

ws.onopen = () => {
    tambahkanPesan('Terhubung ke server.');
    const nama = prompt("Masukkan nama Anda:");
    ws.send(nama); // Mengirimkan nama sebagai pesan pertama
};

ws.onmessage = (event) => {
    tambahkanPesan(event.data); // Menampilkan pesan yang diterima
};

ws.onclose = () => {
    tambahkanPesan('Koneksi ke server terputus.');
};

function kirimPesan() {
    const pesan = inputPesan.value.trim();
    if (pesan) {
        ws.send(pesan);
        inputPesan.value = '';
        if (pesan.toLowerCase() === 'exit') {
            ws.close(); // Menutup koneksi jika "exit" diketik
        }
    }
}

function tambahkanPesan(pesan) {
    const elemenBaru = document.createElement('div');
    elemenBaru.textContent = pesan;
    divPesan.appendChild(elemenBaru);
    divPesan.scrollTop = divPesan.scrollHeight; // Scroll otomatis ke bawah
}