#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys, struct, math

def read_chunks(wav_bytes):
    if wav_bytes[0:4] != b'RIFF' or wav_bytes[8:12] != b'WAVE':
        raise ValueError("Pas un fichier RIFF/WAVE valide.")
    pos = 12
    chunks = []
    while pos + 8 <= len(wav_bytes):
        cid = wav_bytes[pos:pos+4]
        csize = struct.unpack('<I', wav_bytes[pos+4:pos+8])[0]
        cstart = pos + 8
        cend = cstart + csize
        if cend > len(wav_bytes): break
        data = wav_bytes[cstart:cend]
        chunks.append((cid, csize, data))
        pos = cend + (csize & 1)  # padding pair
    return chunks

def write_chunks(chunks):
    body = b''.join(
        cid + struct.pack('<I', csize) + data + (b'\x00' if (csize & 1) else b'')
        for (cid, csize, data) in chunks
    )
    riff_size = len(body) + 4  # 'WAVE' + chunks
    return b'RIFF' + struct.pack('<I', riff_size) + b'WAVE' + body

def get_samplerate(fmt_data):
    if len(fmt_data) < 8:
        raise ValueError("Chunk fmt trop court.")
    return struct.unpack('<I', fmt_data[4:8])[0]

def parse_audacity_labels(path):
    labels = []
    with open(path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'): 
                continue
            parts = line.split('\t')
            if len(parts) < 2:
                continue
            start = float(parts[0])
            name = parts[2].strip() if len(parts) >= 3 else ''
            labels.append((start, name))
    return labels

def sec_to_samples(t, sr):
    return int(round(t * sr))

def build_cue_chunk(labels, sr):
    # cue chunk avec dwPosition rempli (nécessaire pour Unreal)
    entries = []
    for i, (t, _) in enumerate(labels, start=1):
        sample_off = sec_to_samples(t, sr)
        entry  = struct.pack('<I', i)           # dwIdentifier
        entry += struct.pack('<I', sample_off)  # dwPosition = position en samples
        entry += b'data'                        # fccChunk
        entry += struct.pack('<I', 0)           # dwChunkStart
        entry += struct.pack('<I', 0)           # dwBlockStart
        entry += struct.pack('<I', sample_off)  # dwSampleOffset
        entries.append(entry)
    payload = struct.pack('<I', len(entries)) + b''.join(entries)
    return (b'cue ', len(payload), payload)

def build_list_adtl_chunk(labels, sr):
    # LIST(adtl) contenant les labels (texte des marqueurs)
    sub = b'adtl'
    for i, (t, name) in enumerate(labels, start=1):
        if not name:
            mm = int(t // 60)
            ss = int(t % 60)
            ms = int(round((t - math.floor(t)) * 1000))
            name = f"Marker {i:03d} @ {mm:02d}:{ss:02d}.{ms:03d}"
        txt = name.encode('utf-8') + b'\x00'
        labl_data = struct.pack('<I', i) + txt
        labl_size = len(labl_data)
        sub += b'labl' + struct.pack('<I', labl_size) + labl_data
        if labl_size & 1:
            sub += b'\x00'
    return (b'LIST', len(sub), sub)

def insert_before_data(chunks, *to_insert):
    out, inserted = [], False
    for (cid, csize, data) in chunks:
        if not inserted and cid == b'data':
            out.extend(to_insert)
            inserted = True
        out.append((cid, csize, data))
    if not inserted:
        out.extend(to_insert)
    return out

def inject_markers(wav_in, labels_path, wav_out):
    with open(wav_in, 'rb') as f:
        raw = f.read()

    chunks = read_chunks(raw)
    fmt_chunk = next((d for (cid, _, d) in chunks if cid == b'fmt '), None)
    if fmt_chunk is None:
        raise ValueError("Chunk 'fmt ' introuvable.")
    sr = get_samplerate(fmt_chunk)

    labels = parse_audacity_labels(labels_path)
    if not labels:
        raise ValueError("Aucun label valide trouvé dans le .txt Audacity.")

    # Supprimer anciens marqueurs
    filtered = []
    for (cid, csize, data) in chunks:
        if cid == b'cue ':
            continue
        if cid == b'LIST' and csize >= 4 and data[:4] == b'adtl':
            continue
        filtered.append((cid, csize, data))

    cue_chunk = build_cue_chunk(labels, sr)
    adtl_chunk = build_list_adtl_chunk(labels, sr)
    inserted = insert_before_data(filtered, cue_chunk, adtl_chunk)

    out_bytes = write_chunks(inserted)
    with open(wav_out, 'wb') as f:
        f.write(out_bytes)

    print(f"✅ {len(labels)} marqueurs injectés dans {wav_out} (SR={sr} Hz)")

def main():
    if len(sys.argv) != 4:
        print("Usage: python inject_markers_unreal.py input.wav labels.txt output.wav")
        sys.exit(1)
    inject_markers(sys.argv[1], sys.argv[2], sys.argv[3])

if __name__ == "__main__":
    main()
