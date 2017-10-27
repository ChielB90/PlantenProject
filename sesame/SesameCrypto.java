package com.example.chileam.sesame;

import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.charset.Charset;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;

/**
 * Created by chileam on 1-6-2016. ESP Crypto
 */
public class SesameCrypto {
    private SecretKey key;
    private Cipher aesCipher;
    private Cipher aesCBCCipher;
    private byte[] sha1Hash;
    private byte[] encryptHash;
    private byte[] pack;

    public SesameCrypto(SecretKey keyESP, IvParameterSpec ivESP) {

        try {
            KeyGenerator generator = KeyGenerator.getInstance("AES");
            generator.init(128);
            key = generator.generateKey();
            aesCipher = Cipher.getInstance("AES");
            aesCipher.init(Cipher.ENCRYPT_MODE, key);

            aesCBCCipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            aesCBCCipher.init(Cipher.ENCRYPT_MODE, keyESP, ivESP);
        }

        catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException | InvalidAlgorithmParameterException e) {
            Log.e("ERROR CRYPT INIT", e.toString());
            System.out.println(e.toString());
        }
        catch (Exception e) {
            Log.e("ERROR MAIN SHIT", e.toString());
        }
    }

    public void hashMSG(String msg) {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-1");
            sha1Hash = digest.digest(msg.getBytes());
            Log.w("hash.length ", String.format("%s", sha1Hash.length));
        }
        catch (NoSuchAlgorithmException e) {
            System.out.println(e.toString());
        }
    }

    public void encrypHASH() {
        try {
            encryptHash = aesCipher.doFinal(sha1Hash);
            Log.w("EmcryptHash.length ", String.format("%s", encryptHash.length));
        }
        catch (IllegalBlockSizeException | BadPaddingException e) {
            System.out.println(e.toString());
        }
    }

    public void packMSG(String msg) {
        ByteArrayOutputStream str = new ByteArrayOutputStream(msg.length() + key.getEncoded().length + encryptHash.length);
        try {
            str.write(msg.getBytes(Charset.defaultCharset()));
            str.write(key.getEncoded());
            str.write(encryptHash);
            pack = str.toByteArray();
            Log.w("PACK text", new String(pack));
            Log.w("PACK Size", String.format("%s", pack.length));
        }
        catch (IOException e) {
            Log.e("ERROR", e.toString());
        }
    }

    public byte[] encryptPack() {
        byte[] result = null;
        try {
            result = aesCBCCipher.doFinal(pack);
        }
        catch (Exception e) {
            Log.e("CRYPTO", e.toString());
            System.out.println(e.toString());
        }
        return result;
    }
}
