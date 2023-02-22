package com.example.smartring;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.squareup.picasso.Picasso;

import java.util.ArrayList;

public class FotosAdaptador extends RecyclerView.Adapter<FotosAdaptador.FotoViewHolder> {

    ArrayList<Foto> fotos;
    Activity activity;

    public FotosAdaptador(ArrayList<Foto> fotos, Activity activity) {
        this.fotos = fotos;
        this.activity = activity;
    }



    @NonNull
    @Override
    public FotoViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.cardview_foto, parent, false);

        return new FotoViewHolder(v);
    }

    @Override
    public void onBindViewHolder(@NonNull FotoViewHolder holder, int position) {

        final Foto foto = fotos.get(position);

        Picasso.get().
                load(foto.getUrl())
                .placeholder(R.drawable.camera)
                .into(holder.imgFoto);

        holder.tvFecha.setText(String.valueOf(foto.getFecha()));

    }

    @Override
    public int getItemCount() {
        return fotos.size();
    }

    public static class FotoViewHolder extends RecyclerView.ViewHolder {

        private ImageView imgFoto;
        private TextView tvFecha;

        public FotoViewHolder(View itemView) {
            super(itemView);

            imgFoto     = (ImageView) itemView.findViewById(R.id.imgFoto);
            tvFecha     = (TextView) itemView.findViewById(R.id.tvFecha);

        }
    }

}
