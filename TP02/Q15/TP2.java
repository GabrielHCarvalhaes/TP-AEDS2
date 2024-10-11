package TP02.Q15;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Scanner;

class Pokemon{

    private int id;
    private int generation;
    private String name;
    private String description;
    private List<String> type;
    private List<String> abilities;
    private double weight;
    private double height;
    private int captureRate;
    private boolean isLegendary;
    private Date captureDate;

    public List<String> getAbilities() {
        return abilities;
    }
    public Date getCaptureDate() {
        return captureDate;
    }
    public int getCaptureRate() {
        return captureRate;
    }
    public String getDescription() {
        return description;
    }
    public int getGeneration() {
        return generation;
    }
    public double getHeight() {
        return height;
    }
    public int getId() {
        return id;
    }
    public boolean getIsLegendary(){
        return isLegendary;
    }
    public String getName() {
        return name;
    }
    public List<String> getType() {
        return type;
    }
    public double getWeight() {
        return weight;
    }
    public void setAbilities(List<String> abilities) {
        this.abilities = abilities;
    }
    public void setCaptureDate(Date captureDate) {
        this.captureDate = captureDate;
    }
    public void setCaptureRate(int captureRate) {
        this.captureRate = captureRate;
    }
    public void setDescription(String description) {
        this.description = description;
    }
    public void setGeneration(int generation) {
        this.generation = generation;
    }
    public void setHeight(double height) {
        this.height = height;
    }
    public void setId(int id) {
        this.id = id;
    }
    public void setIsLegendary(boolean isLegendary) {
        this.isLegendary = isLegendary;
    }
    public void setName(String name) {
        this.name = name;
    }
    public void setType(List<String> type) {
        this.type = type;
    }
    public void setWeight(double weight) {
        this.weight = weight;
    }

    public Pokemon(){

        this.id = 0;
        this.generation = 0;
        this.name = "";
        this.description = "";
        this.type = new ArrayList<>(); 
        this.abilities = new ArrayList<>(); 
        this.weight = 0.0;
        this.height = 0.0;
        this.captureRate = 0;
        this.isLegendary = false;
        this.captureDate = new Date(); 
    }

    // Construtor com parâmetros
    public Pokemon(int id, int generation, String name, String description, List<String> types, List<String> abilities, double weight, double height, int captureRate, boolean isLegendary, Date captureDate) {
        this.id = id;
        this.generation = generation;
        this.name = name;
        this.description = description;
        this.type = types != null ? types : new ArrayList<>(); // Evita valor nulo
        this.abilities = abilities != null ? abilities : new ArrayList<>(); // Evita valor nulo
        this.weight = weight;
        this.height = height;
        this.captureRate = captureRate;
        this.isLegendary = isLegendary;
        this.captureDate = captureDate;
    }

    public void read(String input) throws ParseException{

        //* 0 1    2           3         4      5                  6             0   1   2  3  4     5                      
        //? 1,1,Bulbasaur,Seed Pokémon,grass,poison,"['Overgrow', 'Chlorophyll']" , 6.9,0.7,45,0,05/01/1996

        String[] splitPt1 = input.split("\""); // Separar o colchete
        String[] splitPt2 = splitPt1[0].split(","); // Separa a primeira parte das virgulas

        // Essa parte e para antes do colchete

        this.id = Integer.parseInt(splitPt2[0]);
        this.generation = Integer.parseInt(splitPt2[1]);
        this.name = splitPt2[2];
        this.description = splitPt2[3];
        this.type.add(splitPt2[4]);

        if(splitPt2.length > 5)type.add(splitPt2[5]); // LISTA TA SEPARANDO OS 2 TIPOS

        // Tratar o colchete para depois adicionar ao objeto

        splitPt1[1].replace("[","");
        splitPt1[1].replace("]","");
        abilities.add(splitPt1[1]); // TODAS AS HABILIDADES ESTAO NUMA UNICA STRING E UMA UNICA CELULA DA LISTA

        //* Trabalhar com a parte pos colchete 

        String[] splitPt3 = splitPt1[2].split(",");

        if(splitPt3[1] != null && splitPt3[1].length() > 0) 
        try{this.weight = Double.parseDouble(splitPt3[1]);}
        catch(Exception e){
            this.weight = 0.0;
        }
        
        if(splitPt3[2] != null && splitPt3[1].length() > 0) 
        try{this.height = Double.parseDouble(splitPt3[2]);}
        catch(Exception e){
            this.height = 0.0;
        }
        this.captureRate = Integer.parseInt(splitPt3[3]);
        if(splitPt3[4].equals("0"))this.isLegendary = false;
        else this.isLegendary = true;
        SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy");   
        this.captureDate = sdf.parse(splitPt3[5]);
    }

    public void print() {
        
        System.out.print("[#" + getId() + " -> "+getName() + ": " + getDescription() + " - ");
        if(type.size() == 2)System.out.print("['"+type.get(0)+"', '"+type.get(1)+"'] - ");
        else System.out.print("['"+type.get(0)+"'] - ");
        SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy");
        String data= sdf.format(getCaptureDate());
        System.out.println(abilities.get(0)+" - " + getWeight()+"kg - " + getHeight() + "m - " + getCaptureRate() + "% - " + getIsLegendary() + " - " + getGeneration() + " gen] - "+data);
    }

    public void leiaIds(List<Integer>ids){
        Scanner sc=new Scanner(System.in);
        String input;
        input = sc.nextLine();
        while(!(input.equals("FIM"))){
            ids.add(Integer.parseInt(input)-1);
            input = sc.nextLine();
        }
    }

}

public class TP2 {

    public static void main(String[] args) throws IOException, ParseException {
        
        long startTime = System.currentTimeMillis();
        FileInputStream fstream = new FileInputStream("/tmp/pokemon.csv");
        BufferedReader br = new BufferedReader(new InputStreamReader(fstream));
        Scanner sc = new Scanner(System.in);
        int count = 0;

        String input = br.readLine();
        Pokemon[] pokemons = new Pokemon[801];
        List<Integer> ids = new ArrayList<>();

        input = br.readLine();
        while(input != null) {
            Pokemon tmp = new Pokemon();
            tmp.read(input);
            pokemons[count] = tmp;
            count++;
            input = br.readLine();
        }
        leiaIds(ids);
        String[] pokemonNome = new String[ids.size()];
        pegandoNome(ids, pokemons, pokemonNome);
        int[] idOrd = new int[ids.size()];
        copiandoId(ids, idOrd);
        ordenando(pokemonNome, idOrd, pokemons);
        for(int i = 0; i < 10; i++) {
            pokemons[idOrd[i]].print();
        }
        long endTime = System.currentTimeMillis();
        long executionTime = endTime - startTime;
    }

    public static void leiaIds(List<Integer> ids) {
        Scanner sc = new Scanner(System.in);
        String input;
        input = sc.nextLine();
        while(!(input.equals("FIM"))) {
            ids.add(Integer.parseInt(input) - 1);
            input = sc.nextLine();
        }
    }

    public static void pegandoNome(List<Integer> ids, Pokemon[] pokemons, String[] pokemonNome) {
        for(int i = 0; i < ids.size(); i++) {
            pokemonNome[i] = pokemons[ids.get(i)].getName();
        }
    }

    public static void copiandoId(List<Integer> ids, int[] idsOrd) {
        for(int i = 0; i < ids.size(); i++) {
            idsOrd[i] = ids.get(i);
        }
    }

    public static void ordenando(String[] pokemonNome, int[] idsOrd, Pokemon[] pokemons) {
        int k = 10;  

        // Ordena os primeiros k elementos usando a ordenação por seleção
        for (int i = 0; i < Math.min(k, pokemonNome.length); i++) {
            int minIdx = i;
            for (int j = i + 1; j < pokemonNome.length; j++) {
                // Comparação por nome
                if (pokemonNome[j].compareTo(pokemonNome[minIdx]) < 0) {
                    minIdx = j;
                }
            }

            // Troca os nomes
            String tempNome = pokemonNome[i];
            pokemonNome[i] = pokemonNome[minIdx];
            pokemonNome[minIdx] = tempNome;

            // Troca os IDs correspondentes
            int tempId = idsOrd[i];
            idsOrd[i] = idsOrd[minIdx];
            idsOrd[minIdx] = tempId;
        }

    }
}
