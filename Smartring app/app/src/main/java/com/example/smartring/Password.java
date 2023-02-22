package com.example.smartring;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class Password extends AppCompatActivity {

    private Activity activity;

    private EditText contraseniaActual;
    private EditText nuevaContrasenia;
    private EditText contraseniaConfirmada;

    private FirebaseDatabase database;
    private DatabaseReference passRef;

    private boolean actualizar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_password2);

        contraseniaActual = (EditText) findViewById(R.id.contraseniaActual);
        nuevaContrasenia = (EditText) findViewById(R.id.nuevaContrasenia);
        contraseniaConfirmada = (EditText) findViewById(R.id.contraseniaConfirmada);

        Button btnEnviar = (Button) findViewById(R.id.btnEnviar);

        activity = this;

        database = FirebaseDatabase.getInstance();
        passRef = database.getReference("sensores/password");

        btnEnviar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                confirmarContraseniaNuevaIgual();
            }
        });
    }


    public void confirmarContraseniaNuevaIgual(){
        String nueva = nuevaContrasenia.getText().toString();
        String confirmar = contraseniaConfirmada.getText().toString();
        if(nueva.length() != 6){
            Toast.makeText(getApplicationContext(), "La contraseña debe consistir de 6 caracteres", Toast.LENGTH_SHORT).show();
            return;
        }
        if(nueva.equals(confirmar)){
            actualizarContrasenia(nueva);
        } else {
            Toast.makeText(getApplicationContext(), "La nueva contraseña no es igual en ambos campos", Toast.LENGTH_SHORT).show();
        }
    }

    public void actualizarContrasenia(String nuevaContrasenia){
        String password = contraseniaActual.getText().toString();
        actualizar = true;
        passRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if(actualizar) {
                    String value = snapshot.getValue(String.class);
                    if (value.equals(password)) {
                        passRef.setValue(nuevaContrasenia);
                        actualizar = false;
                        Toast.makeText(getApplicationContext(), "Se actualizó la contraseña", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(getApplicationContext(), "La contraseña ingresada no es correcta", Toast.LENGTH_SHORT).show();
                    }
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(getApplicationContext(), "Ocurrio un problema, por favor intenta de nuevo", Toast.LENGTH_SHORT).show();
            }
        });
    }

}