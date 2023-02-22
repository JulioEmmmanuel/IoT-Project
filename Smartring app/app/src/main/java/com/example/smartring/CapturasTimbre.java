package com.example.smartring;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;

public class CapturasTimbre extends AppCompatActivity {

    private RecyclerView rvFotos;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_capturas_timbre);

        rvFotos = (RecyclerView) findViewById(R.id.rvFotos);
        new FirebaseFotoDatabase("faces", this, rvFotos);

        GridLayoutManager gridLayoutManager = new GridLayoutManager(getBaseContext(), 2);
        rvFotos.setLayoutManager(gridLayoutManager);
    }
}