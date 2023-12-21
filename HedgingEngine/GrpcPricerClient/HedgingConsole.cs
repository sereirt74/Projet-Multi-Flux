using ParameterInfo.JsonUtils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ParameterInfo;
using MarketData;
using HedgingEngine.GrpcClient;
using HedgingEngine.Hedger;
using System.Text.Json.Serialization;
using System.Text.Json;

namespace HedgingEngine
{
    public class HedgingConsole
    {

        public static void Main(string[] args)
        {
            if (args.Length != 3) {
                Console.WriteLine("Only three arguments are required!");
             }
            string jsonTestParamString = File.ReadAllText(args[0]);
            TestParameters testParams = JsonIO.FromJson(jsonTestParamString);
            string marketDataString = File.ReadAllText(args[1]);
            List<DataFeed> dataFeeds = MarketDataReader.ReadDataFeeds(marketDataString);
            PricerClient pricerClient = new PricerClient(testParams);
            Hedger.Hedger hedger = new Hedger.Hedger(dataFeeds.ToArray(), pricerClient);
            hedger.Hedge(dataFeeds);
            string outputDataString = JsonIO.ToJson(hedger.OutputDataList);
            File.WriteAllText(args[2], outputDataString);
        }


    }
}
