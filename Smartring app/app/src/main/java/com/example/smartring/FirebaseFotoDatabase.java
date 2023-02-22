package com.example.smartring;

import android.app.Activity;
import android.content.Context;
import android.net.Uri;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.ListResult;
import com.google.firebase.storage.StorageMetadata;
import com.google.firebase.storage.StorageReference;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

public class FirebaseFotoDatabase {

    private FirebaseStorage storage;
    private StorageReference storageRef;
    private StorageReference movRef;

    private FirebaseAuth mAuth;

    private ArrayList<String> urls;
    private ArrayList<String> dates;
    private ArrayList<Foto> fotos;

    private RecyclerView rvFotos;

    private int totalFotos = 0;
    private int numUrl = 0;
    private int numDate = 0;

    Activity activity;

    public FirebaseFotoDatabase(String path, Activity activity, RecyclerView rvFotos){

        storage = FirebaseStorage.getInstance();
        storageRef = storage.getReference();
        movRef = storageRef.child(path);

        mAuth = FirebaseAuth.getInstance();

        urls = new ArrayList<String>();
        dates = new ArrayList<String>();
        fotos = new ArrayList<Foto>();

        this.activity = activity;
        this.rvFotos = rvFotos;

        authenticate();

    }


    private void authenticate(){
        mAuth.signInWithEmailAndPassword("juliomeza2510@outlook.com", "julio123")
                .addOnCompleteListener(activity, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if(task.isSuccessful()){
                            Toast.makeText(activity, "Conexión establecida con éxito", Toast.LENGTH_SHORT).show();
                            Toast.makeText(activity, "Esperando al servidor...", Toast.LENGTH_SHORT).show();
                            getData();
                        } else {
                            Toast.makeText(activity, "Ocurrió un error", Toast.LENGTH_SHORT).show();
                        }
                    }
                });
    }

    private void getData(){
        movRef.listAll()
                .addOnSuccessListener(new OnSuccessListener<ListResult>() {
                    @Override
                    public void onSuccess(ListResult listResult) {
                        totalFotos = listResult.getItems().size();
                        for(StorageReference item : listResult.getItems()){
                            getUrl(item.getPath());
                            getDate(item.getPath());
                        }
                    }
                })
                .addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {
                        Toast.makeText(activity, "Ocurrió un error", Toast.LENGTH_SHORT).show();;
                    }
                });
    }

    private void getUrl(String path){
        storageRef.child(path).getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
            @Override
            public void onSuccess(Uri uri) {
                urls.add(uri.toString());
                numUrl++;
                if(numUrl == totalFotos && numDate == totalFotos){
                    setFotos();
                    inicializarAdaptador(fotos);
                }
            }
        }).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception e) {
                return;
            }
        });
    }

    private void getDate(String path){
        storageRef.child(path).getMetadata().addOnSuccessListener(new OnSuccessListener<StorageMetadata>() {
            @Override
            public void onSuccess(StorageMetadata storageMetadata) {
                DateFormat obj = new SimpleDateFormat("yyyy-MM-dd \n HH:mm:ss");
                Date res = new Date(storageMetadata.getCreationTimeMillis());
                dates.add(obj.format(res));
                numDate++;
                if(numUrl == totalFotos && numDate == totalFotos){
                    setFotos();
                    inicializarAdaptador(fotos);
                }
            }
        }).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception e) {
                return;
            }
        });
    }

    private void setFotos(){
        int max = Math.min(totalFotos, 15);

        for(int i = totalFotos-1; i>=totalFotos-max; i--){
            fotos.add(new Foto(urls.get(i), dates.get(i)));
        }
    }

    private void inicializarAdaptador(ArrayList<Foto> fotos){
        FotosAdaptador adaptador = new FotosAdaptador(fotos, activity);
        rvFotos.setAdapter(adaptador);
    }


}
