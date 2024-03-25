import ROOT

# Open the ROOT file
file = ROOT.TFile.Open("output_tot.root")
if not file or file.IsZombie():
    print("Error: Unable to open the ROOT file or the file is corrupted.")
    exit(1)

# Access the folder and tree
folder = file.Get("muon")
if not folder:
    print("Error: Unable to find the folder 'muon' in the ROOT file.")
    exit(1)

tree = folder.Get("StandAloneEvents")
if not tree:
    print("Error: Unable to find the tree 'StandAloneEvents' in the folder 'muon'.")
    exit(1)

# Create histograms to store sum and count of probe_isTrkMatch for each bin of genmu2_phi
hist_sum = ROOT.TH1F("hist_sum", "Sum of probe_isTrkMatch vs genmu2_phi", 62, -3.2, 3.2)
hist_count = ROOT.TH1F("hist_count", "Count of events vs genmu2_phi", 62, -3.2, 3.2)

# Fill the histograms
for event in tree:
    if 25 <= event.genmu2_pt <= 60 and -1.5 <= event.genmu2_eta <= 0.2 and event.genMass >= 70 and event.genMass <= 115 and (event.genmu1_pt > 27) and (event.genmu1_charge == -event.genmu2_charge)  and (event.HLT_IsoMu24_v==1 or event.HLT_Mu50_v==1):
        hist_sum.Fill(event.genmu2_phi, event.probe_isTrkMatch)
        hist_count.Fill(event.genmu2_phi, 1)

# Create a TGraph to store the mean value of probe_isTrkMatch vs genmu2_phi
graph = ROOT.TGraph()

# Compute the mean and fill the TGraph
for bin in range(1, hist_sum.GetNbinsX() + 1):
    bin_center = hist_sum.GetBinCenter(bin)
    sum_value = hist_sum.GetBinContent(bin)
    count_value = hist_count.GetBinContent(bin)
    mean_value = sum_value / count_value if count_value != 0 else 0  # Avoid division by zero
    point_index = graph.GetN()
    graph.SetPoint(point_index, bin_center, mean_value)

# Create a canvas and draw the points
canvas = ROOT.TCanvas("canvas", "Mean of probe_isTrkMatch vs genmu2_phi", 800, 600)
graph.SetMarkerStyle(ROOT.kFullCircle)
graph.SetMarkerSize(1.0)  # Adjust the marker size here (2.0 is just an example)
graph.Draw("AP")  # "A" for points, "P" to draw axes

# Customize the plot if needed
graph.SetTitle("Mean of probe_isTrkMatch vs genmu2_phi")
graph.GetXaxis().SetTitle("genmu2_phi")
graph.GetYaxis().SetRangeUser(0.2, 1.1)  # Adjust the y-axis range here
graph.GetYaxis().SetTitle("Mean of probe_isTrkMatch")

# Add text to the canvas to indicate the constraints
latex = ROOT.TLatex()
latex.SetTextSize(0.03)
latex.SetTextAlign(12)
latex.DrawLatexNDC(0.2, 0.3, "genmu2_pt: 25 GeV < pt < 60 GeV")
latex.DrawLatexNDC(0.2, 0.2, "genmu2_eta: -1.5 < eta < 0.2")

# Add legend
legend = ROOT.TLegend(0.7, 0.8, 0.9, 0.9)
legend.SetFillStyle(0)
legend.SetTextSize(0.03)
legend.AddEntry("", "DY2023postBPix", "")
legend.Draw()


# Save the plot as an image file if needed
canvas.SaveAs("mean_probe_isTrkMatch_vs_genmu2_phi.png")

# Optionally, display the plot
canvas.Draw()
