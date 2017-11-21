#!/usr/bin/env wolframscript
(* ::Package:: *)

SetDirectory["E:\\Google Drive\\ULG\\M0\\git\\pa_p2\\bench_list"]
p1=ListLinePlot[Import["results.txt","Table"],PlotTheme->"Detailed",FrameLabel->{"Taille du labyrinthe","Temps en secondes"},ImageSize->Medium,PlotStyle->Blue]
SetDirectory["E:\\Google Drive\\ULG\\M0\\git\\pa_p2\\bench_tree"]
p2=ListLinePlot[Import["results.txt","Table"],PlotTheme->"Detailed",FrameLabel->{"Taille du labyrinthe","Temps en secondes"},ImageSize->Medium,PlotStyle->Red]
Show[p1,p2,PlotRange->All]
p3=ListLinePlot[Import["results2.txt","Table"],PlotTheme->"Detailed",FrameLabel->{"Taille du labyrinthe","Temps en secondes"},ImageSize->Medium,PlotStyle->Red]














